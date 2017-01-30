# mime-types(1)

## NAME

**mime-types** - example of how to use regex API

## SYNOPSIS

**mime-types** [OPTIONS] REGEX

## DESCRIPTION

**mime-types(1)** is a example of how to use **regex(3)** API.

## OPTIONS

**-v or --verbose**
	Turn on verbose messages.

**-D or --debug**
	Turn on debug messages.

**-V or --version**
	Display the version.

**-h or --help**
	Display this message.

## EXIT STATUS

Exits success if command-line is valid; exits failure otherwise.

## EXAMPLES

Get all video H26x video MIME types

	$ ./mime-types "^video/H26.*"
	video/H261
	video/H263
	video/H263-1998
	video/H263-2000
	video/H264
	video/H264-RCDO
	video/H264-SVC
	video/H265

## AUTHOR

Written by Gaël PORTAY *gael.portay@gmail.com*

## COPYRIGHT

Copyright (c) 2016-2017 Gaël PORTAY

This program is free software: you can redistribute it and/or modify it under
the terms of the MIT License.
