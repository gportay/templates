# casync(1)

## NAME

casync - a webstore for casync with building facilities

## SYNOPSIS

**make** [TARGET...]

## TARGETS

Cleaning targets

**clean**
	Remove most generated files but keep the chunck store and casync local build

**mrproper**
	Remove all generated files + chunk store

**distclean**
	mrproper + remove casync local build

Generic targets:

**all**
	Run the chunk webstore (http://localhost/)

**httpd**
	fedora-httpd

**fedora-httpd**
	Run fedora copy of nginx

**debian-httpd**
	Run debian copy of nginx

Build targets

**\*.catar**
	Create the .catar archive of the /

**\*.caidx**
	Create the .caidx index and its associated chunk store of the /

**\*.caibx**
	Create the .caibx index and its associated chunk store of the file

Test targets

**shell**
	Run shell in Docker

**fedora-httpd**
	Run shell in fedora docker image

**debian-httpd**
	Run shell in debian docker image

**extract\_\*.catar**
	Extract the contents of .catar archive to .

**extract\_\*.caidx**
	Extract the contents of .catar index to .

**extract\_\*.caibx**
	Extract the contents of .caidx index to .extracted

**list\_\*.catar**
	List all files and directories from .catar archive

**list\_\*.caidx**
	List all files and directories from .caidx index

## AUTHOR

Written by Gaël PORTAY *gael.portay@gmail.com*

## COPYRIGHT

Copyright (C) 2019 Gaël PORTAY

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 2.1 of the License, or
(at your option) any later version.

## SEE ALSO

[make(1)]

[make(1)]: https://linux.die.net/man/1/make
