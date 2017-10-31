# templates

Here remains some of templates and examples of source files I developped.
Mostly *C/C++*, *Shell/Bash* and *Makefile*.

## C/C++

[addressbook](c++/protobuf/README.md) - example of how to use protobuf API and
create a Makefile

[ueventcat](c/netlink/README.md) - concatenate UEVENT(s) to standard output

[daemonize](c/daemonize/README.md) - create daemon process

[epoll](c/epoll/README.md) - example of how to use epoll API

[getopt](c/getopt/README.md) - example of how to use getopt\_long function

[hexdump](c/hexdump/README.md) - display buffer content in hexadecimal and
ascii

[ini](c/parser/ini/README.md) - simple C ini parser without context

[line](c/parser/line/README.md) - simple C line parser without context

[pexec](c/pipe/README.md) - execute file in an interprocess channel

[publisher,epoll-publisher,subscriber](c/zmq/README.md) - simple ZMQ publisher
and subscriber applets

[mime-types](c/regex/README.md) - example of how to use regex API

[uevent](c/parser/uevent/README.md) - simple C kobject/uevent parser without
context

[verrevcmp](pkg/version-lt/README.md) - comparison of version numbers

[zombize](c/zombize/README.md) - create zombie process

[zpc-publisher,zpc-subscriber](c/zmq+protobuf/README.md) - simple ZMQ publisher
and subscriber applets using protobuf messages

## MAKEFILE

[asm-setup](android/asm) - asm installer written in make

[udev](android/udev) - udev rules installer for oneplus devices

[openssl](openssl/genkeys/README.md) - provides implicite rules to generate
openssl RSA keys

[foreach](makefile/foreach/README.md) - build a repository of archive using
foreach function

## SHELL/BASH

[amount](shell/amount/README.md) - serial-disk auto-mounter

[autoextract.sh](shell/autoextract/README.md) - make a tar auto-extractable

[cleanup.sh](shell/cleanup/README.md) - how to implement a shell destructor
script in shell

[deb-xtar](shell/deb/README.md) - restore files from nested data.tar.gz archive
of a debian package

[docker-shell](docker/shell/README.md) - run a user shell in a new container
with cwd bind mounted

[ini-parser.bash](bash/ini-parser/README.md) - an ini-style parser written in
bash for bash

[prompt_yes_no](shell/prompt/README.md) - prompt user for a yes/no reply

[version-lt.sh](pkg/version-lt/README.md) - prints the lower version

## SED

[protodec-c.sed](sed/protodec-c/README.md) - decompile protobuf C header

## AUTHOR

Written by Gaël PORTAY *gael.portay@gmail.com*

## COPYRIGHT

Copyright (c) 2015-2017 Gaël PORTAY

This program is free software: you can redistribute it and/or modify it under
the terms of the MIT License.
