#
# Copyright 2020 Gaël PORTAY
#
# SPDX-License-Identifier: LGPL-2.1-or-later
#

.PHONY: all
all: qstring

qstring: CXXFLAGS += -fPIC
qstring: CXXFLAGS += -I /usr/include/qt
qstring: CXXFLAGS += -I /usr/include/qt/QtCore
qstring: LDLIBS += -lQt5Core

.PHONY: test
test: PATH:=$(CURDIR):$(PATH)
test: qstring 
	qstring "/dev/sda1@/efi/grub/grub.cfg" "/dev/sda1" "/dev" "/dev/"

.PHONY: clean
clean:
	rm -f qstring
