# ini_parse(3)

## NAME

**ini_parse** - simple C ini parser without context

## SYNOPSIS

**#include "**_ini.h_**"**

typedef **int** _ini\_section\_cb\_t_**(char \*, void \*);**

typedef **int** _ini\_property\_cb\_t_**(char \*, char \*, void \*);**

**int ini_parse(int** _fd_**, ini_section_cb_t** \*_sec\_cb_
**,ini_property_cb_t** \*_prop\_cb_ **, void** \*_data_**);**

## DESCRIPTION

**ini_parse** is a simple *C* *ini* parser function without context.

It reads and parses data from file descriptor _fd_. Then it calls back either
_sec\_cb_ function when a _section_ is met or _prop\_cb_ when a _property_ is
met.

## EXAMPLES

[test-ini.c](test-ini.c) is a good example of how to use the **ini_parse**
function.

## AUTHOR

Written by Vivien DIDELOT *vivien.didelot@savoirfairelinux.com* and
Gaël PORTAY *gael.portay@savoirfairelinux.com*

## COPYRIGHT

Copyright (c) 2017 Vivien DIDELOT and Gaël PORTAY

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.
