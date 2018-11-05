# Template of codes

Here remain many examples of source files I have developed for years.

Those examples of code are mostly written in *C/C++*, *Shell/Bash*, and
*make/Makefile*. Those languages are the foundations of *low-level* and *system*
development.

Some of these examples, as [zpc-publisher,zpc-subscriber] or [docker-shell],
have served as *proof of concept*. The primary goal was to get familiar with new
libraries; sometimes to mix libraries together as [ProtoBuf] and [ZeroMQ] with
[zpc-publisher,zpc-subscriber]. Latter, some have become *standalone* projects:
[docker-shell] gave birth to [dosh].

Others examples, such as [getopt] or [hexdump], are good templates to get
started with a brand new project or to reuse code.

## C/C++

[addressbook](c++/protobuf/README.md) - example of how to use protobuf API and
create a Makefile

[alarm](c/alarm/README.md) - example of how to use alarm function

[blkpg-part](c/ioctl/README.md) - partition table and disk geometry handling
utility

[daemonize](c/daemonize/README.md) - create daemon process

[epoll](c/epoll/README.md) - example of how to use epoll API

[getopt](c/getopt/README.md) - example of how to use getopt\_long function

[hexdump](c/hexdump/README.md) - display buffer content in hexadecimal and
ascii

[ini](c/parser/ini/README.md) - simple C ini parser without context

[line](c/parser/line/README.md) - simple C line parser without context

[mime-types](c/regex/README.md) - example of how to use regex API

[pexec](c/pipe/README.md) - execute file in an interprocess channel

[publisher,epoll-publisher,subscriber](c/zmq/README.md) - simple ZMQ publisher
and subscriber applets

[sigwaitinfo](c/sigwaitinfo/README.md) - example of how to use sigwaitinfo
function

[sqlite-blob](c/sqlite/README.md) - example of how to insert/replace or select a
blob in sqlite

[uevent](c/parser/uevent/README.md) - simple C kobject/uevent parser without
context

[ueventcat](c/netlink/README.md) - concatenate UEVENT(s) to standard output

[verrevcmp](pkg/version-lt/README.md) - comparison of version numbers

[zombize](c/zombize/README.md) - create zombie process

[zpc-publisher,zpc-subscriber](c/zmq+protobuf/README.md) - simple ZMQ publisher
and subscriber applets using protobuf messages

## MAKEFILE

[asm-setup](android/asm) - asm installer written in make

[udev](android/udev) - udev rules installer for oneplus devices

[openssl](openssl/genkeys/README.md) - provides implicit rules to generate
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

Copyright (c) 2015-2018 Gaël PORTAY

This program is free software: you can redistribute it and/or modify it under
the terms of the MIT License.

[docker-shell]: docker/shell/README.md
[getopt]: c/getopt/README.md
[hexdump]: c/hexdump/README.md
[zpc-publisher,zpc-subscriber]: c/zmq+protobuf/README.md
[ZeroMQ]: http://zeromq.org/
[ProtoBuf]: https://developers.google.com/protocol-buffers/
[dosh]: https://github.com/gazoo74/dosh/
