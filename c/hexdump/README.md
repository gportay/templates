# hexdump(3)

## NAME

**hexdump** - display buffer content in hexadecimal and ascii

## SYNOPSIS

**#include "**_hexdump.h_**"**

**static void hexdump(unsigned int** _address_**, const unsigned char**
\*_buf_**, unsigned int** _bufsize_**);**

**static void fhexdump(FILE** \*_f_**, unsigned int** _address_
**, const unsigned char** \*_buf_**, unsigned int** _bufsize_**);**

**static int** _DUMPHEADER_ **= 1;**

**static int** _DUMPFOOTER_ = **1;**

**static int** _DUMPADDR_ = **1;**

**static unsigned int** _ROWSIZE_ **= 0x10;**

**static char** _EMPTYBYTE_ = **' ';**

## DESCRIPTION

**hexdump** imitates what **hexdump(1)** does when it is invoked along with
_-C_ argument. The function **hexdump(3)** writes output to stdout, the standard
output stream; **fhexdump(3)** writes output to the given output stream.

The **hexdump** function dumps the _bufsize_ bytes of input _buf_ by rows,
first in hexadecimal and then in _ASCII_. Non-printable characters are replaced
by a dot `_._'.

The size of dumped rows is set using _ROWSIZE_ global. This variable is set by
default to _0x10_ to display 16 bytes per row.

When _buf_ does not fill the row, **hexdump** prints an empty space _`` '_ to
feed the row. This character is set using the global _EMPTYBYTE_.

if _DUMPADDR_ is non-null, each lines is prefixed by the _address_ + current
_row_.

if _DUMPFOOTER_ is non-null, *hexdump* displays the address beyong the data of
as footer (i.e. _address_ + _bufsize_).

if _DUMPHEADER_ is non-null, *hexdump* displays the following header at first
line

	@address: 00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f

Note that _@address:_ and the number of bytes depends on both _DUMPADDR_ and
_ROWSIZE_ values.

## EXAMPLES

[hexdump.c](hexdump.c#L172-L204) is a good example of how to use the
**hexdump** function.

## AUTHOR

Written by Gaël PORTAY *gael.portay@gmail.com*

## COPYRIGHT

Copyright (c) 2015-2017 Gaël PORTAY

This program is free software: you can redistribute it and/or modify it under
the terms of the MIT License.
