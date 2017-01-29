# asm-setup(1)

## NAME

[asm-setup](asm-setup) - asm installer written in make

## SYNOPSIS

**asm-setup** [PREFIX=/usr] [DESTDIR=] [TARGET=all]...

## TARGET

### asm-setup(1)

**asm-setup** [un]install [PREFIX=/usr] [DESTDIR=]

to [un]install binary to system.

**asm-setup [un]install-user**

to install binary to current user.

**asm-setup** android-screen-monitor

to fetch sources.

**asm-setup** clean

to remove temporary files.

**asm-setup** help

to display this message.

**asm-setup** Makefile

to create a Makefile symlink and then run make frontend.

### make(1)

**make** all

to download sources to current directory.

## ENVIRONMENT

**PREFIX**
	The prefix [default: _/usr_].

**DESTDIR**
	To install offline.

## DESCRIPTION

[asm-setup](asm-install) is a _Makefile_ based installer. It is an example of
how to provide an alternative to _shell_ scripting to do things. The _Makefile_
must be executable and its shebang sets to _#!/usr/bin/make -f_.

Even if this example is not appropriate, using **make(1)** have several
advantages. It makes things easier to parallelize tasks and to avoid redoing
tasks from scratch when they are done.

## EXAMPLES

### System install

Install for all users

	# asm-setup install

or with super-user privileges

	$ sudo asm-setup install

### User install

Install for current user only

	$ asm-setup install DESTDIR=~ PREFIX=

or using install-user alias

	$ asm-setup install-user

## AUTHOR

Written by Gaël PORTAY *gael.portay@gmail.com*

## COPYRIGHT

Copyright (c) 2016-2017 Gaël PORTAY

This program is free software: you can redistribute it and/or modify it under
the terms of the MIT License.
