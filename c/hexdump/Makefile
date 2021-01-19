#
# Copyright 2015,2017,2021 Gaël PORTAY
#
# SPDX-License-Identifier: MIT
#

.PHONY: all
all: hexdump

hexdump.o: hexdump.h hexdump.c

.PHONY: install
install: PREFIX ?= /usr/local
install:
	install -D -m644 hexdump.h $(DESTDIR)$(PREFIX)/include/hexdump.h

.PHONY: clean
clean:
	rm -f hexdump hexdump.o
