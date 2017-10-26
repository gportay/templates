# zombize(1)

## NAME

**zombize** - create zombie process

## SYNOPSIS

**zombize** PATH [ARG...]

## DESCRIPTION

**zombize(1)** creates a zombie process using the given *PATH* command and
optional *ARG*.

It **fork(3)** and **execv(3)** but does not **wait(3)** for the child process
to terminate, creating zombie process.

## EXIT STATUS

Exits success if fork succeed; exits failure otherwise.

## AUTHOR

Written by Gaël PORTAY *gael.portay@savoirfairelinux.com*

## COPYRIGHT

Copyright (c) 2017 Gaël PORTAY

This program is free software: you can redistribute it and/or modify it under
the terms of the MIT License.

## SEE ALSO

fork(3), exec(3)
