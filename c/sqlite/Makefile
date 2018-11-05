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

override CFLAGS += $(shell pkg-config sqlite3 --cflags)
override LDLIBS += $(shell pkg-config sqlite3 --libs)

.PHONY: all
all: override CFLAGS += -g -Wall -Wextra -Werror
all: sqlite-blob

.PHONY: clean
clean:
	rm -f sqlite-blob database.sql

