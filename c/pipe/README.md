# pexec(3)

## NAME

**pexec** - execute file in an interprocess channel

## SYNOPSIS

**#include "**_pexec.h_**"**

**int pexecl(pid_t** \*_pid_**, const char** \*_path_**, const char** \*_arg_
**, ...);**

**int pexeclp(pid_t** \*_pid_**, const char** \*_file_**, const char** \*_arg_
**, ...);**

**int pexecle(pid_t** \*_pid_**, const char** \*_path_**, const char** \*_arg_
**, ...);**

**int pexecv(pid_t** \*_pid_**, const char** \*_path_**, char** \* **const**
_argv[]_**);**

**int pexecvp(pid_t** \*_pid_**, const char** \*_file_**, char** \* **const**
_argv[]_**);**

**int pexecve(pid_t** \*_pid_**, const char** \*_path_**, char** \* **const**
_argv[]_**, char** \* **const** _envp[]_**);**

#ifdef _GNU_SOURCE

**pid_t pexecvpe(const char** \*_file_**, char** \* **const** _argv[]_
**, char** \* **const** _envp[]_**);**

#endif

## DESCRIPTION

The **pexec(3)** familly of functions combines **pipe(3p)**, **fork(2)** and
**exec(3)** functions.

These functions open a **pipe(3p)** between the caller and the new created
process.

The process is **fork(2)'ed** and its image is **exec(3)'ed** with the content
of _path_ or _file_ given in parameter using _argv_ as argument and _envp_ as
environment.

For a more thorough description of the **pexec(3)** functions familly, please
refers theirs **exec(3)** equivalents.

## RETURN VALUE

Upon successful completion, these functions return the file descriptor of the
created pipe; if not NULL, the PID of the child process is set to _pid_.
Otherwise, these functions shall return −1 and set errno to indicate the error.

## AUTHOR

Written by Gaël PORTAY *gael.portay@savoirfairelinux.com*

## COPYRIGHT

Copyright (c) 2017 Gaël PORTAY

This program is free software: you can redistribute it and/or modify it under
the terms of the MIT License.

## SEE ALSO

**pipe(3p)**, **fork(2)**, **exec(3)**
