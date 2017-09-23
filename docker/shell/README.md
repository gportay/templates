# docker-shell(1)

_Important_: [docker-shell](docker-shell) was a proof of concept. Please see
[dosh](https://github.com/gazoo74/dosh) for a full featured utility.

## NAME

[docker-shell](docker-shell.1.adoc) - run a user shell in a new container with
pwd bind mounted

## DESCRIPTION

[docker-shell](docker-shell) runs the _command_ process in a new container;
using the current _user_, with _pwd_ bind mounted.

## LINKS

Check for the [man-page](docker-shell.1.adoc) and its
[examples](docker-shell.1.adoc#examples).

Also, here is an extra example that build the documentation

	$ echo FROM ubuntu >Dockerfile
	$ echo RUN apt-get update && apt-get install -y asciidoctor >>Dockerfile

	$ cat Dockerfile
	FROM ubuntu
	RUN apt-get update && apt-get install -y asciidoctor

	$ docker-shell -c asciidoctor -b manpage -o - docker-shell.1.adoc | gzip -c - >docker-shell.1.gz
	Sending build context to Docker daemon 2.048 kB
	Step 1 : FROM ubuntu
	 ---> f49eec89601e
	Step 2 : RUN apt-get update && apt-get install -y asciidoctor
	 ---> Using cache
	 ---> a21052a106d9
	Step 3 : RUN groupadd --non-unique --gid 1000 gportay
	 ---> Using cache
	 ---> 9d5ca9684940
	Step 4 : RUN useradd  --non-unique --gid 1000 --uid 1000 --create-home --home-dir /home/gportay --shell /bin/bash gportay
	 ---> Using cache
	 ---> ced062433e33
	Successfully built ced062433e33


	$ man ./docker-shell.1.gz

Enjoy!

## BUGS

Report bugs at *https://github.com/gazoo74/templates/issues*

## AUTHOR

Written by Gaël PORTAY *gael.portay@savoirfairelinux.com*

## COPYRIGHT

Copyright (c) 2017 Gaël PORTAY

This program is free software: you can redistribute it and/or modify it under
the terms of the MIT License.
