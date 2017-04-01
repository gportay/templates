#!/bin/sh
#
# Copyright 2017 Gaël PORTAY <gael.portay@savoirfairelinux.com>
#
# Licensed under the MIT license.
#

set -e

mkdir -p /root/
cd /root/
[ -e openssl.cnf ] || cp /etc/ssl/openssl.cnf ./

mkdir -p demoCA/
cd demoCA/
mkdir -p certs newcerts
[ -e serial      ] || echo "00" >serial
[ -e crlnumber   ] || echo "00" >crlnumber
[ -e index       ] || touch index.txt
cd ..

openssl genrsa -out ca.key 2048
openssl req -new -x509 -days 3650 \
	-subj "/C=CA/ST=Quebec/L=Montreal/O=DemoCA Ltd/OU=IT/CN=www.example.org" \
	-key ca.key -out ca.crt

openssl genrsa -out client.key 2048
openssl req -config openssl.cnf \
	-subj "/C=CA/ST=Quebec/L=Montreal/O=DemoCA Ltd/OU=IT/CN=client@example.org" \
        -new -key client.key -out client.csr

openssl ca -batch -config openssl.cnf -days 3650 \
	-in client.csr -out client.crt \
	-keyfile ca.key -cert ca.crt -policy policy_anything

openssl pkcs12 -export -in client.crt -inkey client.key -certfile ca.crt -out client.p12

cd /etc/lighttpd/
openssl req -newkey rsa:2048 \
	-subj "/C=CA/ST=Quebec/L=Montreal/O=DemoCA Ltd/OU=IT/CN=www.example.org" \
	-x509 -keyout server.pem -out server.pem -days 365 -nodes
chmod 400 /etc/lighttpd/server.pem

mkdir -p /etc/lighttpd/ssl/
cat /root/ca.key /root/ca.crt >/etc/lighttpd/ssl/www.example.org.pem
