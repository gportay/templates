#!/usr/bin/make -f
#
# Copyright 2016-2017 Gaël PORTAY <gael.portay@gmail.com>
#
# Licensed under the MIT license.
#

CXXFLAGS += -I. -std=c++11
LDFLAGS  += -lstdc++
SRC := $(wildcard *.cpp)
OBJ := $(SRC:.cpp=.o)

all: poller

poller: $(OBJ)

clean:
	rm -f $(OBJ) $(OBJ:.o=)

