#
# Copyright 2019 Gaël PORTAY
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

.PHONY: all
all:

.PHONY: install
install:
	install -D -m644 echo.socket ~/.config/systemd/user/echo.socket
	install -D -m 644 echo@.service ~/.config/systemd/user/echo@.service

.PHONY: uninstall
uninstall:
	rm -f ~/.config/systemd/user/echo.socket
	rm -f ~/.config/systemd/user/echo@.service

.PHONY: start
start:
	systemctl --user $@ echo.socket

.PHONY: start
stop:
	systemctl --user $@ echo.socket "echo@*.service"

.PHONY: status
status:
	systemctl --user $@ echo.socket "echo@*.service"

.PHONY: daemon-reload
daemon-reload:
	systemctl --user $@

.PHONY: test
test:
	nc localhost 1234
