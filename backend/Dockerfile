FROM cloudposse/apache

WORKDIR /backend


RUN apt-get update && \
    apt-get -y install apache2-dev build-essential


COPY apache2.conf /etc/apache2/apache2.conf
COPY git /var/www/html/.git
COPY module module


RUN apxs2 -i -a -c module/mod_ping.c && \
    chmod 644 /usr/lib/apache2/modules/mod_ping.so && \
    apt-get -y remove build-essential


RUN a2enmod ping && a2disconf security && apachectl restart

