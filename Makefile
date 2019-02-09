#
# Copyright 2019 Gaël PORTAY
#
# SPDX-License-Identifier: LGPL-2.1-or-later
#

MAKEFILES := $(wildcard */Makefile */*/Makefile */*/*/Makefile)

.PHONY: all
all:
	@echo "You may want to run: $(MAKE) world" >&2

.PHONY: world
world:
	for f in $(MAKEFILES); do \
		$(MAKE) -C "$${f%/*}" all; \
	done

.PHONY: clean distclean mrproper
clean distclean mrproper:
	-for f in $(MAKEFILES); do \
		$(MAKE) -C "$${f%/*}" $@; \
	done

