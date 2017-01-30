# Makefile

## NAME

[Makefile](Makefile) - provides implicite rules to generate openssl RSA keys

## SYNOPSIS

**make** [KEYNAME=\<USERNAME\>\_rsa] [PRIVKEY=\<KEYNAME\>.pem]
[PUBKEY=<KEYNAME>.pub] [TARGET=all]...

## TARGET

**make** all

Encrypt/decrypt and sign/verify a text message. Generate RSA keys if they are
missing.

**make** keys

Generate RSA keys only.

**make** clean

Clean crypted/decrypted and signed text messages.

**make** mrproper

Clean crypted/decrypted and signed text messages, and removes RSA keys.

## ENVIRONMENT

**KEYNAME** The _RSA_ key name [default: \<USERNAME\>\_rsa]

**PRIVKEY** The _private_ key name [default: \<KEYNAME\).pem]

**PUBKEY** The _public_ key name [default: \<KEYNAME\>.pub]

## DESCRIPTION

This _Makefile_ provides implicite rules to generate _RSA_ keys using
**openssl(1)**.

Those implicit [rules](Makefile#L27-L52) are freely available for use, reuse,
adaptation, and sharing in any Makefile. You may also credit myself.

## EXAMPLES

By default, **make(1)** will generate _RSA_ keys to _encrypt_ and _decrypt_ a
text message. It also _signs_ and _verify_ it.

	$ make KEYNAME=example
	openssl genrsa -aes256                              -out example.pem
	Generating RSA private key, 2048 bit long modulus
	................................................................................................+++
	..+++
	e is 65537 (0x10001)
	Enter pass phrase for example.pem:
	Verifying - Enter pass phrase for example.pem:
	openssl    rsa                           -in example.pem     -out example.pub -outform PEM -pubout
	Enter pass phrase for example.pem:
	writing RSA key
	openssl rsautl -encrypt -inkey example.pub -pubin -in message.txt     -out message.txt.enc
	openssl rsautl -decrypt -inkey example.pem        -in message.txt.enc
	Enter pass phrase for example.pem:
	This is my private message!
	openssl dgst   -sha1 -sign   example.pem              message.txt         >message.txt.sig
	Enter pass phrase for example.pem:
	openssl dgst   -sha1 -verify example.pub -signature   message.txt.sig      message.txt
	Verified OK
	rm message.txt.sig message.txt.enc

Do some cleanup to remove everything.

	$ make mrproper
	rm -f message.*
	rm -f *.pub* *.der *.pem*

## AUTHOR

Written by Gaël PORTAY *gael.portay@gmail.com*

## COPYRIGHT

Copyright (c) 2016-2017 Gaël PORTAY

This program is free software: you can redistribute it and/or modify it under
the terms of the MIT License.
