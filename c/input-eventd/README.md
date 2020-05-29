# input-eventd(8)

## NAME

**input-eventd** - a simple input-event daemon reader.

## SYNOPSIS

**input-eventd** [OPTIONS] DEVICE

## DESCRIPTION

**input-eventd(8)** is a simple daemon that read [input-events] from *DEVICE*
and forwards them to script [/usr/share/input-event.action].

## OPTIONS

**-t or --timeout SECOND**
	Set time-out in seconds [default=-1].

**-s or --script**
	Set script [default=/usr/share/input-event.action].

**-v or --verbose**
	Turn on verbose messages.

**-V or --version**
	Display the version.

**-h or --help**
	Display this message.

## AUTHOR

Written by Gaël PORTAY *gael.portay@gmail.com*

## COPYRIGHT

Copyright (c) 2015, 2020 Gaël PORTAY
All rights reserved.

Redistribution and use in source and binary forms are permitted provided that
the above copyright notice and this paragraph are duplicated in all such forms
and that any documentation, advertising materials, and other materials related
to such distribution and use acknowledge that the software was developed by the
<organization>. The name of the <organization> may not be used to endorse or
promote products derived from this software without specific prior written
permission.  THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

[input-events]: https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/include/uapi/linux/input-event-codes.h
[/usr/share/input-event.action]: input-event.action
