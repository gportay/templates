# cleanup.sh

## NAME

[cleanup.sh](cleanup.sh) - how to implement a shell destructor script in shell

## SYNOPSIS

**cleanup.sh** [CMD [ARGS...]]

## DESCRIPTION

**cleanup.sh** is a demo script of how to run a script destructor in shell.

Destructors consist in cleanup things the script has done (e.g. temporary
files...)

It uses **trap(1p)** to setup a function to callback when script exits.

## EXAMPLES

Without argument, the script runs its usage and exit success

	$ ./cleanup.sh && echo okay
	1234: ./cleanup.sh: Running usage...
	Usage: ./cleanup.sh [CMD [ARGS]]
	1234: ./cleanup.sh: usage returned 0!
	okay

Here is an example of how to run an _echo_ command that succeed

	$ ./cleanup.sh echo "Hello, World!" && echo okay
	1234: ./cleanup.sh: Running echo Hello, World!...
	Hello, World!
	1234: ./cleanup.sh: echo returned 0!
	okay

Here is an example of how to run a command that fails (_false_)

	$ ./cleanup.sh false && echo okay
	1234: ./cleanup.sh: Running false...
	1243: ./cleanup.sh: false returned 1!

Note: _1234_ is the pid.

## AUTHOR

Written by Gaël PORTAY *gael.portay@gmail.com*

## COPYRIGHT

Copyright (c) 2015-2017 Gaël PORTAY

This program is free software: you can redistribute it and/or modify it under
the terms of the MIT License.
