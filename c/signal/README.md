# signal(1)

## NAME

**signal** - example of how to use sigwaitinfo function

## SYNOPSIS

**signal** [OPTIONS]

## DESCRIPTION

**signal(1)** is a good template to begin with a new _CLI_ in _C_, based on a
basic skeleton using **sigwaitinfo(3p)** to wait for queued signals as main
loop.

## OPTIONS

**-v or --verbose**
	Turn on verbose messages.

**D or --debug**
	Turn on debug messages.

**-h or --help**
	Display this message.

**-V or --version**
	Display the version.

## EXIT STATUS

Exits success if **SIGINT** or **SIGTERM** are queued; exits failure otherwise.

## EXAMPLES

See [signal.c](signal.c#L133-L195).

## AUTHOR

Written by Gaël PORTAY *gael.portay@savoirfairelinux.com*

## COPYRIGHT

Copyright (C) 2017 Savoir-Faire Linux Inc.

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 2 of the License, or (at your option) any later
version.

## SEE ALSO

**sigwaitinfo(3p)**
