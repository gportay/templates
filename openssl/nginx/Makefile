#
#  Copyright (C) 2018 Gaël PORTAY
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

DESTDIR ?=

C  ?= CA
ST ?= Quebec
L  ?= Montreal
O  ?= L'electron du libre
OU ?= dev

.SECONDARY:

.PHONY: all
all: cacert.pem

private newcerts:
	mkdir $@

serial crlnumber:
	echo "00" >$@

index.txt index.txt.attr:
	touch $@

private/cakey.pem: | private
	openssl genrsa -out $@ 2048

cacert.pem: CN ?= www.portay.io
cacert.pem: private/cakey.pem
	openssl req -new -x509 -days 3650 \
		    -subj "/C=$(C)/ST=$(ST)/L=$(L)/O=$(O)/OU=$(OU)/CN=$(CN)" \
		    -key $< -out $@

openssl.cnf: | newcerts index.txt serial
	sed -e '/^dir/adir		= $$ENV::PWD' \
	    /etc/ssl/openssl.cnf >$@

%.key.pem:
	openssl genrsa -out $@ 2048

%.csr.pem: openssl.cnf %.key.pem
	openssl req -config openssl.cnf \
		    -subj "/C=$(C)/ST=$(ST)/L=$(L)/O=$(O)/OU=$(OU)/CN=$*" \
		    -new -key $*.key.pem -out $@

%.cert.pem: openssl.cnf private/cakey.pem cacert.pem %.csr.pem
	openssl ca -batch -config openssl.cnf -days 3650 \
	           -keyfile private/cakey.pem -cert cacert.pem -policy policy_anything \
	           -in $*.csr.pem -out $@

%.p12: openssl.cnf cacert.pem %.cert.pem %.key.pem
	openssl pkcs12 -export -in $*.cert.pem -inkey $*.key.pem -certfile cacert.pem -out $@

crl.pem: openssl.cnf index.txt.attr crlnumber private/cakey.pem cacert.pem
	openssl ca -config openssl.cnf \
	           -keyfile private/cakey.pem -cert cacert.pem \
	           -gencrl -out $@

%_revoke: %
	openssl ca -config openssl.cnf \
	           -revoke $*

.PHONY: root
root: root_selfsigned
%_selfsigned:
	openssl req -x509 -days 3650 \
		    -subj "/C=$(C)/ST=$(ST)/L=$(L)/O=$(O)/OU=$(OU)/CN=$*" \
	            -newkey rsa\:2048 -keyout $*.key-crypted.pem \
	            -out $*.cert.pem

%.key-crypted.pem: %.key.pem
	openssl rsa -aes256 -in $*.key.pem -out $*.key-crypted.pem

%.key.pem: %.key-crypted.pem
	openssl rsa -in $*.key-crypted.pem -out $*.key.pem

%.key-crypted.pem_dump:
	openssl rsa -in $*.key-crypted.pem -text -noout

%.key.pem_dump:
	openssl rsa -in $*.key.pem -text -noout

%.cert.pem_dump:
	openssl x509 -in $*.cert.pem -text -noout

%.csr.pem_dump:
	openssl req -in $*.csr.pem -text -noout

.PHONY: crl.pem_dump
crl.pem_dump:
	openssl crl -in crl.pem -noout -text

%.p12_dump:
	openssl pkcs12 -info -in $*.p12

%.pem_to_der:
	openssl x509 -outform der -in $*.pem -out $*.der

%.der_to_pem:
	openssl x509 -inform der -in $*.der -out $*.pem

.PHONY: install
install: SHELL=/bin/bash
install:
	install -d $(DESTDIR)/etc/ssl/
	cp -a private newcerts $(DESTDIR)/etc/ssl/
	sed -e '/^dir		= $$ENV::PWD/d' openssl.cnf >$(DESTDIR)/etc/ssl/openssl.cnf
	install index.txt{,.attr} serial $(DESTDIR)/etc/ssl/

.PHONY: clean
clean: SHELL=/bin/bash
clean:
	rm -rf openssl.cnf newcerts private index.txt{,.attr} serial crlnumber *.old *.pem *.p12

