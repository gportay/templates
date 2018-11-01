#
# Copyright 2016 Gaël PORTAY <gael.portay@gmail.com>
#
# Licensed under the MIT license.
#

PBSRC := $(wildcard *.proto)
PBOBJ := $(PBSRC:.proto=.pb.cc)

SRC := $(wildcard *.cc)
SRC += $(PBOBJ)

OBJ := $(SRC:.cc=.o)

.SECONDARY:: $(PBOBJ)

all:: addressbook

addressbook: $(OBJ)

addressbook.o: addressbook.pb.h

ifeq (,$(shell which protoc 2>/dev/null))
all:: /usr/local/bin/protoc

/usr/local/bin/protoc:
	make -f protobuf.mk
	sudo make -f protobuf.mk install

LDFLAGS += -L/usr/local/lib
endif

CC = g++
CXXFLAGS += -I. -std=c++11
LDFLAGS += -L/usr/local/lib
LDFLAGS += $(shell pkg-config --cflags --libs protobuf) -lstdc++

%.pb.cc %.pb.h: %.proto
	protoc --cpp_out=$(<D) $<

debug:
	@echo SRC: $(SRC)
	@echo PBSRC: $(PBSRC)
	@echo PBOBJ: $(PBOBJ)

clean:
	rm -f addressbook $(OBJ) $(PBOBJ:.pb.cc=.pb.*)

