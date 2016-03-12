#
# Copyright 2016 Gaël PORTAY <gael.portay@gmail.com>
#
# Licensed under the MIT license.
#

all: protobuf/build/src/protoc

protobuf/build/src/protoc: protobuf/build/Makefile
	( cd $(@D) && make )

protobuf/build/Makefile: protobuf/configure
	install -d $(@D)/
	( cd $(@D) && ../configure )

protobuf/configure: protobuf/configure.ac
	( cd $(@D) && ./autogen.sh )

protobuf/configure.ac:
	git clone https://github.com/google/protobuf $(@D)

install:
	( cd protobuf/build && make install )

