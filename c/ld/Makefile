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

override CFLAGS += -g -Wall -Wextra -Werror

.PHONY: all
all: dlrun library.so
	LD_LIBRARY_PATH="." PATH=".:$$PATH" dlrun entrypoint library.so one two three
	./library.so

dlrun: override LDFLAGS += -ldl
library.so: override LDFLAGS += -Wl,-pie -Wl,-E

#library.c:38:6: error: return type of ‘main’ is not ‘int’ [-Werror=main]
# void main()
#      ^~~~
#cc1: all warnings being treated as errors
library.o: override CFLAGS += -Wno-main

.PHONY: clean
clean:
	rm -f dlrun library.so library.o

%.so: override CFLAGS += -fPIC
%.so: override LDFLAGS += -Wl,-shared
%.so: %.o
	$(LINK.o) $^ $(LOADLIBES) $(LDLIBS) -o $@

