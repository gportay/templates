#
# Copyright 2020 Gaël PORTAY
#
# SPDX-License-Identifier: LGPL-2.1-or-later
#

.PHONY: all
all: qflags

qflags: CXXFLAGS += -fPIC
qflags: CXXFLAGS += -I /usr/include/qt
qflags: CXXFLAGS += -I /usr/include/qt/QtCore
qflags: LDLIBS += -lQt5Core

.PHONY: test
test: PATH:=$(CURDIR):$(PATH)
test: qflags
	qflags RequiredPartition NoBlockIOProtocol LegacyBIOSBootable 8

.PHONY: clean
clean:
	rm -f qflags
