#
# Copyright 2016 Gaël PORTAY <gael.portay@gmail.com>
#
# Licensed under the MIT license.
#

KEYNAME	?= $(shell whoami)_rsa
PRIVKEY	?= $(KEYNAME).pem
PUBKEY	?= $(KEYNAME).pub

.SECONDARY: $(PRIVKEY) $(PUBKEY)

all: keys

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

clean:

mrproper: clean
	rm -f *.pub* *.der *.pem*

