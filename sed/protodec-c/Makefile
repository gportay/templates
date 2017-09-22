#
# Copyright (c) 2017 Gaël PORTAY <gael.portay@savoirfairelinux.com>
#
# This program is free software; you can redistribute it and/or modify it under
# the terms of the GNU General Public License version 3 as published by the
# Free Software Foundation.
#

PREFIX ?= /usr/local

.PHONY: all
all:

.PHONY: doc
doc: protodec-c.1.gz

.PHONY: install
install:
	install -d $(DESTDIR)$(PREFIX)/bin/
	install -m 755 protodec-c $(DESTDIR)$(PREFIX)/bin/protodec-c

.PHONY: install-doc
install-doc:
	install -d $(DESTDIR)$(PREFIX)/share/man/man1/
	install -m 644 protodec-c.1.gz $(DESTDIR)$(PREFIX)/share/man/man1/

.PHONY: uninstall
uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/protodec-c

.PHONY: uninstall-doc
uninstall-doc:
	rm -f $(DESTDIR)$(PREFIX)/share/man/man1/protodec-c.1.gz

.PHONY: check
check: protodec-c
	shellcheck $^

.PHONY: clean
clean:
	rm -f protodec-c.1.gz

.PHONY: test
test: simple.pb-c.h
	sed -nf protodec-c.sed $<

%.pb-c.h %.pb-c.c: %.proto
	protoc-c --c_out=. $<

%.1: %.1.adoc
	asciidoctor -b manpage -o $@ $<

%.gz: %
	gzip -c $^ >$@

