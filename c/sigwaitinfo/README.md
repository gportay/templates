# sigwaitinfo(1)

## NAME

**sigwaitinfo** - example of how to use sigwaitinfo function

## SYNOPSIS

**sigwaitinfo** [OPTIONS]

## DESCRIPTION

**sigwaitinfo(1)** is a good template to begin with a new _CLI_ in _C_, based on
a basic skeleton using **sigwaitinfo(3p)** as main loop.

## OPTIONS

**-h or --help**
	Display this message.

**-V or --version**
	Display the version.

## EXIT STATUS

Exits success if **SIGTERM** is queued; exits failure otherwise.

## AUTHOR

Written by Gaël PORTAY *gael.portay@savoirfairelinux.com*

## COPYRIGHT

Copyright (C) 2018 Savoir-Faire Linux Inc.

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free
Software Foundation, either version 2.1 of the License, or (at your option) any
later version.

## SEE ALSO

**sigwaitinfo(3p)**
