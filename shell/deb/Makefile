#
# Copyright (c) 2017 Gaël PORTAY <gael.portay@savoirfairelinux.com>
#
# This program is free software: you can redistribute it and/or modify it under
# the terms of the MIT License.
#

PREFIX ?= /usr/local

.PHONY: all
all:

.PHONY: doc
doc: deb-xtar.1.gz

.PHONY: install
install:
	install -d $(DESTDIR)$(PREFIX)/bin/
	install -m 755 deb-xtar $(DESTDIR)$(PREFIX)/bin/
	ln -sf deb-xtar $(DESTDIR)$(PREFIX)/bin/deb-control-xtar

.PHONY: install-doc
install-doc:
	install -d $(DESTDIR)$(PREFIX)/share/man/man1/
	install -m 644 deb-xtar.1.gz $(DESTDIR)$(PREFIX)/share/man/man1/

.PHONY: uninstall
uninstall:
	for bin in deb-xtar deb-control-xtar; do \
		rm -f $(DESTDIR)$(PREFIX)/bin/$$bin; \
	done

.PHONY: uninstall-doc
uninstall-doc:
	rm -f $(DESTDIR)$(PREFIX)/share/man/man1/deb-xtar.1.gz

.PHONY: check
check: deb-xtar
	shellcheck $^

.PHONY: clean
clean:
	rm -f deb-xtar.1.gz

%.1: %.1.adoc
	asciidoctor -b manpage -o $@ $<

%.gz: %
	gzip -c $^ >$@

