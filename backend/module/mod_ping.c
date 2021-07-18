
#include <stdio.h>
#include "httpd.h"
#include "http_config.h"
#include "http_protocol.h"
#include "ap_config.h"


static int util_read(request_rec *r, const char **rbuf, apr_off_t *size)
{
    /*~~~~~~~~*/
    int rc = OK;
    /*~~~~~~~~*/

    if((rc = ap_setup_client_block(r, REQUEST_CHUNKED_ERROR))) {
        return(rc);
    }

    if(ap_should_client_block(r)) {

        /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
        char         argsbuffer[HUGE_STRING_LEN];
        apr_off_t    rsize, len_read, rpos = 0;
        apr_off_t length = r->remaining;
        /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

        *rbuf = (const char *) apr_pcalloc(r->pool, (apr_size_t) (length + 1));
        *size = length;
        while((len_read = ap_get_client_block(r, argsbuffer, sizeof(argsbuffer))) > 0) {
            if((rpos + len_read) > length) {
                rsize = length - rpos;
            }
            else {
                rsize = len_read;
            }

            memcpy((char *) *rbuf + rpos, argsbuffer, (size_t) rsize);
            rpos += rsize;
        }
    }
    return(rc);
}


static int ping_handler(request_rec *r)
{
    apr_off_t   size;
    const char  *buffer;

    if (util_read(r, &buffer, &size) != OK)
    {
        return HTTP_NOT_FOUND;
    }

    if (size <= 0)
        return HTTP_NOT_FOUND;

    char url[800];
    // Command injection & buffer overflow
    sprintf(url, "ping -c1 -s1 %s > /dev/null 2>&1", buffer);

    int x = system(url);

    ap_rprintf(r, "Result: %d\n", x);

    if (x >= 0)
        ap_rprintf(r, "Ping successful!");
    else
        ap_rprintf(r, "Ping failed!");

    return OK;
}

static void ping_register_hooks(apr_pool_t *p)
{
    ap_hook_handler(ping_handler, NULL, NULL, APR_HOOK_MIDDLE);
}

/* Dispatch list for API hooks */
module AP_MODULE_DECLARE_DATA ping_module = {
    STANDARD20_MODULE_STUFF,
    NULL,                  /* create per-dir    config structures */
    NULL,                  /* merge  per-dir    config structures */
    NULL,                  /* create per-server config structures */
    NULL,                  /* merge  per-server config structures */
    NULL,                  /* table of config file commands       */
    ping_register_hooks  /* register hooks                      */
};

