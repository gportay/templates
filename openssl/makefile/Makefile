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

.SILENT: help
help:
	echo "help: display this message"
	echo ""
	echo "cakey.pem,cacert.pem[=all]: Generate a new CA private key and self-signed certificate."
	echo "                            Example: $(MAKE) [CN=www.portay.io]"
	echo ""
	echo "%.key.pem: Generate a private key."
	echo "%.cert.pem: Generate a CA-signed certificate."
	echo "%.csr.pem: Generate a certificate signing request (CSR)."
	echo ""
	echo "%.p12: Convert a PEM certificate file and a private key to PKCS#12."
	echo "       Example: $(MAKE) gael.portay@gmail.com.p12"
	echo ""
	echo "%_revoke: Revoke a PEM certificate."
	echo "%         Example: $(MAKE) gael.portay@gmail.com.p12"
	echo "%         Example: $(MAKE) newcerts/00.pem"
	echo ""
	echo "crl.pem: Generate the certificate revocation lists (CRL)."
	echo "%        Example: $(MAKE) crl.pem"
	echo ""
	echo "%.key-crypted.pem: Add passphrase to private key."
	echo "%.key.pem: Remove passphrase from private key."
	echo ""
	echo "%.key-crypted.pem_dump,%.key.pem_dump: Dump (crypted) private key."
	echo "%.cert.pem_dump: Dump certificate."
	echo "%.csr.pem_dump: Dump certificate signing request (CSR)."
	echo "crl.pem_dump: Dump certificate revocation lists (CRL)."
	echo "%.p12_dump: Dump PKCS#12."
	echo "%.pem_to_der: Convert a PEM file to DER."
	echo "%.der_to_pem: Convert a DER file to PEM."
	echo ""

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

%.csr.pem: %.key.pem
	openssl req -subj "/C=$(C)/ST=$(ST)/L=$(L)/O=$(O)/OU=$(OU)/CN=$*" \
		    -new -key $*.key.pem -out $@

%.cert.pem: openssl.cnf private/cakey.pem cacert.pem %.csr.pem
	openssl ca -batch -config openssl.cnf -days 3650 \
	           -keyfile private/cakey.pem -cert cacert.pem -policy policy_anything \
	           -in $*.csr.pem -out $@

%.p12: cacert.pem %.cert.pem %.key.pem
	openssl pkcs12 -export -in $*.cert.pem -inkey $*.key.pem -certfile cacert.pem -out $@

.PHONY: crl.pem
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
	for dir in private newcerts; do \
		if test -d $$dir; then cp -a $$dir $(DESTDIR)/etc/ssl/; fi ;\
	done
	if test -e openssl.cnf; then sed -e '/^dir		= $$ENV::PWD/d' openssl.cnf >$(DESTDIR)/etc/ssl/openssl.cnf; fi
	for file in cacert.pem index.txt{,.attr} serial; do \
		if test -e $$file; then install $$file $(DESTDIR)/etc/ssl/; fi ;\
	done
	cp Makefile $(DESTDIR)/etc/ssl/

.PHONY: clean
clean: SHELL=/bin/bash
clean:
	rm -rf openssl.cnf newcerts private index.txt{,.attr} serial crlnumber *.old *.pem *.p12

