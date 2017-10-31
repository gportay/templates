# uevent_parse(3)

## NAME

**uevent_parse** - simple C kobject/uevent parser without context

## SYNOPSIS

**#include "**_uevent.h_**"**

typedef **int** _uevent\_event\_cb\_t_**(char \*, char \*, void \*);**

typedef **int** _uevent\_variable\_cb\_t_**(char \*, void \*);**

**int uevent_parse(int** _fd_**, uevent_event_cb_t** \*_evt\_cb_
**, uevent_variable_cb_t** \*_var\_cb_ **, void** \*_data_**);**

## DESCRIPTION

**uevent_parse** is a simple *C* *kobject/uevent* parser function without
context.

It reads and parses data from file descriptor _fd_. Then it calls back either
_evt\_cb_ function when an _event_ is met or _var\_cb_ function when a
_variable_ is met.

## EXAMPLES

[test-uevent.c](test-uevent.c) is a good example of how to use the
**uevent_parse** function.

## AUTHOR

Written by Gaël PORTAY *gael.portay@savoirfairelinux.com*

## COPYRIGHT

Copyright (c) 2017 Gaël PORTAY

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.
