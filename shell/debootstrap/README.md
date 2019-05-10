# debootstrap-shell(1)

## NAME

**debootstrap-shell** - command interpreter (shell) for debootstrap

## SYNOPSIS

**debootstrap-shell** [--arch arch] [-aCefnuvxIimqVEb] [+aCefnuvxIimqVEb] [-o option\_name] [+o option_name] [command\_file [argument ...]]

**debootstrap-shell** [--arch arch] -c [-aCefnuvxIimqVEb] [+aCefnuvxIimqVEb] [-o option\_name] [+o option_name] command\_string [command\_name [ argument ...]]

**debootstrap-shell** [--arch arch] -s [-aCefnuvxIimqVEb] [+aCefnuvxIimqVEb] [-o option\_name] [+o option\_name] [argument ...]

## DESCRIPTION

**debootstrap-shell(1)** runs on top of [dash(1)] in a [chroot(1)] build by
[debootstrap(1)].

For a more thorough description of [dash(1)], please refers to its help.

## OPTIONS

**--arch ARCH**
	Set architecture [amd64 arm64].
	Defaults to host.
	_Note_: Given to [debootstrap(1)].

**-v, --version**
	Displays version information.

**-h, --help**
	Displays this help.

## AUTHOR

Written by Gaël PORTAY *gael.portay@gmail.com*

## COPYRIGHT

Copyright (C) 2019 Gaël PORTAY

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.

## SEE ALSO

[chroot(1)], [dash(1)], [debootstrap(1)]

[chroot(1)]: https://linux.die.net/man/1/chroot
[dash(1)]: https://linux.die.net/man/1/dash
[debootstrap(1)]: https://linux.die.net/man/8/debootstrap
