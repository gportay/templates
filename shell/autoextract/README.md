# autoextract.sh

## NAME

[autoextract.sh](autoextract.sh) - make a tar auto-extractable

## DESCRIPTION

**autoextract.sh** is a _shell_ chunck script that makes _TAR_ files
auto-extractable.

Prepend the _TAR_ file with **autoextract.sh** chunk, make it executable and the
result is now auto-extractable using any _shells_.

## EXAMPLE

Create an archive.

	$ tar czf data.tar.gz /etc/passwd /etc/group
	tar: Removing leading `/' from member names

Create auto-extractable file and make it executable.

	$ cat autoextract.sh data.tar.gz >data.sh
	$ chmod +x data.sh

Auto-extract data.

	$ ./data.sh
	$ find .
	.
	./data.sh
	./etc
	./etc/passwd
	./etc/group
	./data.tar.gz

## AUTHOR

Written by Gaël PORTAY *gael.portay@savoirfairelinux.com*

## COPYRIGHT

Copyright (c) 2016-2017 Gaël PORTAY

This program is free software: you can redistribute it and/or modify it under
the terms of the MIT License.
