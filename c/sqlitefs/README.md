# sqlitefs(1)

## NAME

**sqlitefs** - example of how to use FUSE using in a SQLite database filesystem

## SYNOPSIS

**sqlitefs** [OPTIONS] MOUNTPOINT

## DESCRIPTION

**sqlitefs(1)** mounts a virtual filesystem based on SQLite a in Userspace using
FUSE.

## OPTIONS

General options:

**-o opt,[opt...]**
	mount options

**-h   --help**
	print help

**-V   --version**
	print version

FUSE options:

**-d   -o debug**
	enable debug output (implies -f)

**-f**
	foreground operation

**-s**
	disable multi-threaded operation


**-o allow_other**
	allow access to other users

**-o allow_root**
	allow access to root

**-o auto_unmount**
	auto unmount on process termination

**-o nonempty**
	allow mounts over non-empty file/dir

**-o default_permissions**
	enable permission checking by kernel

**-o fsname=NAME**
	set filesystem name

**-o subtype=NAME**
	set filesystem type

**-o large_read**
	issue large read requests (2.4 only)

**-o max_read=N**
	set maximum size of read requests


**-o hard_remove**
	immediate removal (don't hide files)

**-o use_ino**
	let filesystem set inode numbers

**-o readdir_ino**
	try to fill in d_ino in readdir

**-o direct_io**
	use direct I/O

**-o kernel_cache**
	cache files in kernel

**-o [no]auto_cache**
	enable caching based on modification times (off)

**-o umask=M**
	set file permissions (octal)

**-o uid=N**
	set file owner

**-o gid=N**
	set file group

**-o entry_timeout=T**
	cache timeout for names (1.0s)

**-o negative_timeout=T**
	cache timeout for deleted names (0.0s)

**-o attr_timeout=T**
	cache timeout for attributes (1.0s)

**-o ac_attr_timeout=T**
	auto cache timeout for attributes (attr_timeout)

**-o noforget**
	never forget cached inodes

**-o remember=T**
	remember cached inodes for T seconds (0s)

**-o nopath**
	don't supply path if not necessary

**-o intr**
	allow requests to be interrupted

**-o intr_signal=NUM**
	signal to send on interrupt (10)

**-o modules=M1[:M2...]**
	names of modules to push onto filesystem stack

**-o max_write=N**
	set maximum size of write requests

**-o max_readahead=N**
	set maximum readahead

**-o max_background=N**
	set number of maximum background requests

**-o congestion_threshold=N**
	set kernel's congestion threshold

**-o async_read**
	perform reads asynchronously (default)

**-o sync_read**
	perform reads synchronously

**-o atomic_o_trunc**
	enable atomic open+truncate support

**-o big_writes**
	enable larger than 4kB writes

**-o no_remote_lock**
	disable remote file locking

**-o no_remote_flock**
	disable remote file locking (BSD)

**-o no_remote_posix_lock**
	disable remove file locking (POSIX)

**-o [no_]splice_write**
	use splice to write to the fuse device

**-o [no_]splice_move**
	move data while splicing to the fuse device

**-o [no_]splice_read**
	use splice to read from the fuse device

Module options:

[iconv]

**-o from_code=CHARSET**
	original encoding of file names (default: UTF-8)

**-o to_code=CHARSET**
	new encoding of the file names (default: UTF-8)

[subdir]

**-o subdir=DIR**
	prepend this directory to all paths (mandatory)

**-o [no]rellinks**
	transform absolute symlinks to relative

## AUTHOR

Written by Gaël PORTAY *gael.portay@savoirfairelinux.com*

## COPYRIGHT

Copyright (C) 2018 Savoir-Faire Linux Inc.

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free
Software Foundation, either version 2.1 of the License, or (at your option) any
later version.

## SEE ALSO

**fuser(1)**, **fusermount(1)**
