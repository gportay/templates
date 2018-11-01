# addressbook(1)

## NAME

**addressbook** - example of how to use protobuf API and create a Makefile

## SYNOPSIS

**addressbook** [OPTIONS] NAME ID

**addressbook** --decode

## DESCRIPTION

**addressbook(1)** is the application of the protobuf
[addressbook](https://developers.google.com/protocol-buffers/docs/cpptutorial)
example from Google.

The [Makefile](Makefile) is the interresting part. It adds _implicit rules_ for
[proto](Makefile#L36-L37) files (i.e. _%.pb.cc_).

## OPTIONS

**-d or --decode**
	Decode from stdin.

**--email STRING**
	Set e-mail.

**--phone STRING**
	Set phone.

**-D or --debug**
	Turn on debug messages.

**-h or --help**
	Display this message.

**-v or --version**
	Display the version.

## EXIT STATUS

Exits success if command-line is valid; exits failure otherwise.

## AUTHOR

Written by Gaël PORTAY *gael.portay@gmail.com*

## COPYRIGHT

Copyright (c) 2016-2017 Gaël PORTAY

This program is free software: you can redistribute it and/or modify it under
the terms of the MIT License.
