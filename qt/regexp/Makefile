#
# Copyright 2020 Gaël PORTAY
#
# SPDX-License-Identifier: LGPL-2.1-or-later
#

.PHONY: all
all: qregularexpression

qregularexpression: CXXFLAGS += -fPIC
qregularexpression: CXXFLAGS += -I /usr/include/qt
qregularexpression: CXXFLAGS += -I /usr/include/qt/QtCore
qregularexpression: LDLIBS += -lQt5Core

.PHONY: test
test: PATH:=$(CURDIR):$(PATH)
test: qregularexpression
	qregularexpression "^/dev/\\w+@[a-zA-Z0-9./_+-]+$$" "/dev/sda1@/efi/grub/grub.cfg"
	qregularexpression "^[0-9a-fA-F]{8,8}-[0-9a-fA-F]{4,4}-[0-9a-fA-F]{4,4}-[0-9a-fA-F]{4,4}-[0-9a-fA-F]{12,12}$$" "C12A7328-F81F-11D2-BA4B-00A0C93EC93B"

.PHONY: clean
clean:
	rm -f qregularexpression
