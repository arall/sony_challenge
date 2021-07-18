FROM httpd:latest

WORKDIR /usr/local/apache2/htdocs/

COPY public-html/index.html /usr/local/apache2/htdocs/index.html
COPY ./my-httpd.conf /usr/local/apache2/conf/httpd.conf
COPY ./my-httpd-ssl.conf /usr/local/apache2/conf/extra/httpd-ssl.conf
COPY server.key /usr/local/apache2/conf/
COPY server.crt /usr/local/apache2/conf/

# RUN a2enmod proxy proxy_ajp proxy_http rewrite deflate headers proxy_balancer proxy_connect proxy_html

