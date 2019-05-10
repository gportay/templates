#
# Copyright 2019 Gaël PORTAY
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

PREFIX ?= /usr/local

.PHONY: all
all:

.PHONY: install
install:
	install -d $(DESTDIR)$(PREFIX)/bin/
	install -m 755 deboostrap-shell $(DESTDIR)$(PREFIX)/bin/

.PHONY: uninstall
uninstall:
	install -d $(DESTDIR)$(PREFIX)/bin/
	install -m 755 deboostrap-shell $(DESTDIR)$(PREFIX)/bin/

.PHONY: check
check:
	shellcheck deboostrap-shell

.PHONY: tests
tests: SHELL:=/bin/bash
tests:
	sudo $(SHELL) debootstrap-shell -C rootfs-host -c 'apt update && apt install -y file'
	sudo $(SHELL) debootstrap-shell -C rootfs-arm64 --arch arm64 -c 'apt update && apt install -y file'

.PHONY: clean
clean:
	sudo rm -Rf rootfs-*/

# ex: filetype=make
