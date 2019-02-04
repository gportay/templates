# echo<nolink>@.service(8), echo.socket(8)

## NAME

**echo** - an example of an echo socket and service for systemd.

## SYNOPSIS

**[echo.socket(8)]**, **[echo@.service(8)]**

## DESCRIPTION

**[echo@.service(8)]** is an example of an *echo server* service for
**systemd(1)** that is automatically started by the **systemd.socket(5)**
**[echo.socket(8)]** on port *1234*.

## AUTHOR

Written by Gaël PORTAY *gael.portay@gmail.com*

## COPYRIGHT

Copyright (C) 2019 Gaël PORTAY

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.

## SEE ALSO

**[systemd(1)]**, **[systemd.socket(5)]**

[echo@.service(8)]: echo@.service
[echo.socket(8)]: echo.socket
[systemd(1)]: https://www.freedesktop.org/software/systemd/man/systemd.html
[systemd.socket(5)]: https://www.freedesktop.org/software/systemd/man/systemd.socket.html
