# epoll(1)

## NAME

**epoll** - example of how to use epoll API

## SYNOPSIS

**epoll** [OPTIONS]

## DESCRIPTION

**epoll(1)** is a good template to begin with a new _CLI_ in _C_, based on a
basic skeleton using **epoll(2)** API as polling loop.

## OPTIONS

**-t or --timeout SECOND**
	Set time-out in seconds [default=-1].

**-D or --debug**
	Turn on debug messages.

**-h or --help**
	Display this message.

**-v or --version**
	Display the version.

## EXIT STATUS

Exits success if command-line is valid; exits failure otherwise.

## EXAMPLES

See [epoll.c](epoll.c#L149-L204).

## AUTHOR

Written by Gaël PORTAY *gael.portay@gmail.com*

## COPYRIGHT

Copyright (c) 2015-2017 Gaël PORTAY

This program is free software: you can redistribute it and/or modify it under
the terms of the MIT License.
