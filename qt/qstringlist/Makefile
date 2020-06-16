#
# Copyright 2020 Gaël PORTAY
#
# SPDX-License-Identifier: LGPL-2.1-or-later
#

.PHONY: all
all: qstringlist

qstringlist: CXXFLAGS += -fPIC
qstringlist: CXXFLAGS += -I /usr/include/qt
qstringlist: CXXFLAGS += -I /usr/include/qt/QtCore
qstringlist: LDLIBS += -lQt5Core

.PHONY: test
test: PATH:=$(CURDIR):$(PATH)
test: qstringlist
	qstringlist "RequiredPartition NoBlockIOProtocol LegacyBIOSBootable GUID:63"

.PHONY: clean
clean:
	rm -f qstringlist
