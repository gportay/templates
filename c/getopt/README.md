# getopt(1)

## NAME

**getopt** - example of how to use getopt\_long function

## SYNOPSIS

**getopt** [OPTIONS] ARG

## DESCRIPTION

**getopt(1)** is a good template to begin with a new _CLI_ in _C_, based on a
basic skeleton using **getopt\_long(3)** to parse arguments.

## OPTIONS

**-s or --string STRING**
	Set string.

**-n or --number NUMBER**
	Set number.

**-h or --help**
	Display this message.

**-v or --version**
	Display the version.

## EXIT STATUS

Exits success if command-line is valid; exits failure otherwise.

## EXAMPLES

See [getopt.c](getopt.c#L63-L143).

## AUTHOR

Written by Gaël PORTAY *gael.portay@gmail.com*

## COPYRIGHT

Copyright (c) 2015-2017 Gaël PORTAY

This program is free software: you can redistribute it and/or modify it under
the terms of the MIT License.
