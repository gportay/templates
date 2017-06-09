#
# Copyright (C) 2017  Vivien Didelot
#                     Gaël Portay
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

CFLAGS += -Werror -Wextra -Wall -g

.PHONY: all
all: test-ini

ini.o: ini.h

test-ini: test-ini.o ini.o

.PHONY: test
test: test-ini
	./test-ini <config.ini

.PHONY: clean
clean:
	rm -f *.o test-ini
