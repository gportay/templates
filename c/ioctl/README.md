# blkpg-part(1)

## NAME

**blkpg-part** - partition table and disk geometry handling utility

## SYNOPSIS

**blkpg-part** [OPTIONS] add|resize DEVNAME PNO START LENGTH

**blkpg-part** [OPTIONS] delete DEVNAME PNO

## DESCRIPTION

[blkpg-part(1)](blkpg-part.c) adds, resizes and deletes partitions using the
following *BLKPG_ADD_PARTITION*, *BLKPG_DEL_PARTITION* and
*BLKPG_RESIZE_PARTITION* **ioctl(3P)**.

**DEVNAME**: partition name, like sda5 or c0d1p2, to be used in kernel messages.

**PNO**: partition number.

**START**: starting offset in bytes.

**LENGTH**: length in bytes.

## OPTIONS

**-l or --volume-name LABEL**
	Set volume label (Unused at present - linux 4.15).

**-h or --help**
	Display this message.

**-V or --version**
	Display the version.

## EXIT STATUS

Exits success if command-line is valid; exits failure otherwise.

## AUTHOR

Written by Gaël PORTAY *gael.portay@savoirfairelinux.com*

## COPYRIGHT

Copyright (c) 2018 Gaël PORTAY

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, version 2.
