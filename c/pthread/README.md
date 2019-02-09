# pthread-signal(1)

## NAME

**pthread-signal** - example of how to use pthreads and signals

## SYNOPSIS

**pthread-signal** [OPTIONS] [THREAD_ARGS...]

## DESCRIPTION

**pthread-signal(1)** is a good template to begin with a new  CLI in C, based on
a basic skeleton using **sigwaitinfo(3p)** as main loop in main thread that
propagates signals such as *SIGTERM*, *SIGUSR1*, *SIGUSR2* to another thread.

## OPTIONS

**-t or --timeout SEC**
	Set timeout in sec [default=5].
	Note: It is also used to wait for thread to join before returning.

**-v or --verbose**
	Turn on verbose messages.

**-D or --debug**
	Turn on debug messages.

**-h or --help**
	Display this message.

**-V or --version**
	Display the version.

## SIGNALS

**SIGINT**
	Cancel thread.

**SIGTERM**
	Signal thread using Terminate to cause thread to exit.

**SIGUSR1**
	Signal thread using User signal 1.

**SIGUSR2**
	Signal thread using User signal 2.

## AUTHOR

Written by Gaël PORTAY *gael.portay@gmail.com*

## COPYRIGHT

Copyright (C) 2018 Gaël PORTAY

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free
Software Foundation, either version 2.1 of the License, or (at your option) any
later version.

## SEE ALSO

**pthreads(7)**
