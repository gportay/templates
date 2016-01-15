#
# Copyright 2016 Gaël PORTAY <gael.portay@gmail.com>
#
# Licensed under the MIT license.
#

KEYNAME	?= $(shell whoami)_rsa
PRIVKEY	?= $(KEYNAME).pem
PUBKEY	?= $(KEYNAME).pub

.SECONDARY: $(PRIVKEY) $(PUBKEY)

.SILENT: message.txt

all: message.txt.dec message.txt.sig-verify

message.txt:
	echo "This is my private message!" >$@

$(PRIVKEY) $(PUBKEY):

keys: $(PUBKEY)

key-info-%: %.pem
	openssl    rsa                           -in $<     -noout  -text

%.pem:
	openssl genrsa -aes256                              -out $@

%.pem-decrypted: %.pem
	openssl    rsa                           -in $<     -out $@

%.der: %.pem
	openssl    rsa                           -in $<     -out $@ -outform DER

%.pub-rsa: %.pem
	openssl    rsa                           -in $<     -out $@ -RSAPublicKey_out

%.pub: %.pem
	openssl    rsa                           -in $<     -out $@ -outform PEM -pubout

%.enc: $(PUBKEY) %
	openssl rsautl -encrypt -inkey $< -pubin -in $*     -out $@

%.dec: $(PRIVKEY) %.enc
	openssl rsautl -decrypt -inkey $<        -in $*.enc

%.sig: $(PRIVKEY) %
	openssl dgst   -sha1 -sign   $<              $*         >$@

%.sig-verify: $(PUBKEY) %.sig
	openssl dgst   -sha1 -verify $< -signature   $*.sig      $*

decode-%: %.dec

decode-message.txt:

verify-%: %.sig-verify

verify-message.txt:

clean:
	rm -f message.*

mrproper: clean
	rm -f *.pub* *.der *.pem*

