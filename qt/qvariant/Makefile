#
# Copyright 2020 Gaël PORTAY
#
# SPDX-License-Identifier: LGPL-2.1-or-later
#

.PHONY: all
all: qvariant-tonumber

qvariant-tonumber:

qvariant-%: CXXFLAGS += -fPIC
qvariant-%: CXXFLAGS += -I /usr/include/qt
qvariant-%: CXXFLAGS += -I /usr/include/qt/QtCore
qvariant-%: LDLIBS += -lQt5Core

.PHONY: test
test: SHELL=/bin/bash
test: PATH:=$(CURDIR):$(PATH)
test: qvariant-tonumber
	qvariant-tonumber 0 $$((2**0)) $$((2**1)) -$$((2**31)) $$((2**31-1)) $$((2**63)) # $$((2**63-1)) is 0 $$(signed))

.PHONY: clean
clean:
	rm -f qvariant-tonumber
