#
# Copyright 2020 Gaël PORTAY
#
# SPDX-License-Identifier: LGPL-2.1-or-later
#

.PHONY: all
all: qstring-leftright qstring-tonumber

qstring-leftright qstring-tonumber:

qstring-%: CXXFLAGS += -fPIC
qstring-%: CXXFLAGS += -I /usr/include/qt
qstring-%: CXXFLAGS += -I /usr/include/qt/QtCore
qstring-%: LDLIBS += -lQt5Core

.PHONY: test
test: SHELL=/bin/bash
test: PATH:=$(CURDIR):$(PATH)
test: qstring-leftright qstring-tonumber
	qstring-leftright "/dev/sda1@/efi/grub/grub.cfg" "/dev/sda1" "/dev" "/dev/"
	qstring-tonumber 0 $$((2**0)) $$((2**1)) -$$((2**31)) $$((2**31-1)) $$((2**63)) # $$((2**63-1)) is 0 $$(signed))
	qstring-tonumber 0x0 0x1 0x03 0xffffffff 0x1ffffffff 0xffffffffffffffff  0xffff000000000003

.PHONY: clean
clean:
	rm -f qstring-leftright qstring-tonumber
