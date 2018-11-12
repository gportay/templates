#
#  Copyright (C) 2018 Gaël PORTAY
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 2.1 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

override CFLAGS += -g -Wall -Wextra -Werror -pedantic

.PHONY: all
all: loader loader-mt helloworld.so myplugin.so

loader loader-mt: override LDFLAGS += -ldl
loader-mt: override CFLAGS += -pthread

.PHONY: tests mt-tests
tests mt-tests: export override PATH := .:$(PATH)
tests mt-tests: export LD_LIBRARY_PATH = .

tests: all
	loader -v helloworld.so myplugin.so

mt-tests: all
	loader-mt -v helloworld.so myplugin.so

.PHONY: clean
clean:
	rm -f loader loader-mt *.so *.o

%.so: override CFLAGS += -fPIC -I.
%.so: override LDFLAGS += -shared
%.so: plugin.h
%.so: %.o
	$(LINK.o) $^ $(LOADLIBES) $(LDLIBS) -o $@

