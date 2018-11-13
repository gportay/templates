# strtonargv(3)

## NAME

**strtonargv** - convert a string to a list of arguments

## SYNOPSIS

**#include "**_strtonargv.h_**"**

**char **strtonargv(char** \*_dest[]_**, char** \*_src_**, int** \*_n_**);**

**char \*_CFS_ =** _" \t\n"_**;**

## DESCRIPTION

**strtonargv(3)** function converts the string in _src_ to an argument list.

If _dest_ is not _NULL_, **strtonargv(3)** stores the argument list in _dest[]_
up to _*n_ elements and sets _*n_ to the number of elements converted.

If _dest_ is _NULL_, **strtonargv(3)** sets _*n_ to the number of elements that
would have been converted if _dest_ were not _NULL_.

**CFS** is the _Common-Field Separator_ character used to splits a string into
elements.

## RETURN VALUE

The **strtonargv(3)** function sets the argument _n_ to the number of elements
converted and returns the result of the conversion, unless the _dest_ argument
is _NULL_. If the argument _dest_ is _NULL_, **strtonargv(3)** returns _NULL_.
If the argument _n_ is either _NULL_ or negative, **strtonargv(3)** returns
_NULL_ and errno is set to _EINVAL_.

## ERRORS

**EINVAL**
	The given _n_ is either _NULL_ or negative.

## EXAMPLES

[strtonargv.c][example] is a good example of how to use the **strtonargv(3)**
function in two calls; a [first] call to get the number of argument to convert;
a [second] call to convert elements using a stack allocated array of strings.

## AUTHOR

Written by Gaël PORTAY *gael.portay@savoirfairelinux.com*

## COPYRIGHT

Copyright (C) 2018 Savoir-Faire Linux Inc. and Gaël PORTAY

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free
Software Foundation, either version 2.1 of the License, or (at your option) any
later version.

[example]: strtonargv.c#L140-L141,L148-L161
[first]: strtonargv.c#L140-L141
[second]: strtonargv.c#L148-L161
