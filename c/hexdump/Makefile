#
# Copyright 2015,2017,2021 Gaël PORTAY
#
# SPDX-License-Identifier: MIT
#

all: hexdump

hexdump.o: hexdump.h hexdump.c

install: PREFIX ?= /usr/local
install:
	install -D -m644 hexdump.h $(DESTDIR)$(PREFIX)/include/hexdump.h

clean:
	rm -f hexdump hexdump.o
