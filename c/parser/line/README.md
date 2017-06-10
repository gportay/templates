# line_parse(3)

## NAME

**line_parse** - simple C line parser without context

## SYNOPSIS

**#include "**_line.h_**"**

typedef **int** _line\_cb\_t_**(char \*, size_t, void \*);**

**int line_parse(int** _fd_**, line_cb_t** \*_line\_cb_**, void** \*_data_**);**

## DESCRIPTION

**line_parse** is a simple *C* *line* parser function without context.

It reads and parses data from file descriptor _fd_. Then it calls back
_line\_cb_ function when an _end-of-line_ is met.

## EXAMPLES

[test-line.c](test-line.c) is a good example of how to use the **line_parse**
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
