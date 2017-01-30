# version-lt(1)

## NAME

**revvercmp** - comparison of version numbers

**version-lt** - prints the lower version

## SYNOPSIS

### C

**#include "_verrevcmp.h_"**

**int verrevcmp(const char \*_val_, const char \*_ref_);**

**version-lt** [OPTIONS] *VERS* *VERS_REF*

Prints the Lower version of *VERS* and *VERS_REF*.

### Shell

echo "$VERS" "$VERS\_REF" | **verrevcmp.awk**

**version-lt.sh** [OPTIONS] *VERS* *VERS_REF*

Prints the Lower version of *VERS* and *VERS_REF*.

## OPTIONS

**-e or --equal**
	Lower or Equal.

**-q or --quiet**
	Quiet.

**-D or --debug**
	Turn on debug traces.

**-h or --help**
	Display this message.

**-V or --version**
	Display the version.

## EXIT STATUS

Exits success if *VERS* is Lower Than (lt) *VERS_REF*.

## DESCRIPTION

It comes in two versions.

The [C](version-lt.c) version uses [verrecmp](verrevcmp.h#L20-L45) function
extracted from the _Debian_ packaging suite library routines. This version
serves as reference behavior.

The [shell](version-lt.sh) version uses a rewritten version of the **C**
function **int verrevcmp(const char *_val_, const char *_ref_);** in
[awk](verrevcmp.awk).

## EXAMPLES

See [Makefile](Makefile#L17-L42).

## AUTHOR

Written by Gaël PORTAY *gael.portay@gmail.com*

## COPYRIGHT

### VERREVCMP

Copyright (c) 1995 Ian Jackson *iwj10@cus.cam.ac.uk*

### VERSION-LT

Copyright (c) 2016-2017 Gaël PORTAY

This program is free software: you can redistribute it and/or modify it under
the terms of the MIT License.
