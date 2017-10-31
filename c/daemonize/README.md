# daemonize(1)

## NAME

**daemonize** - create daemon process

## SYNOPSIS

**daemonize** PATH [ARG...]

## DESCRIPTION

**daemonize(1)** creates a daemon process using the given *PATH* command and
optional *ARG*.

It **fork(2)** twice, **exec(3)** and **wait(2)** for the child process to
terminate to not create zombie.

## EXIT STATUS

Exits success if child succeed; exits failure otherwise.

## AUTHOR

Written by Gaël PORTAY *gael.portay@savoirfairelinux.com*

## COPYRIGHT

Copyright (c) 2017 Gaël PORTAY

This program is free software: you can redistribute it and/or modify it under
the terms of the MIT License.

## SEE ALSO

**fork(2)**, **exec(3)**, **wait(2)**
