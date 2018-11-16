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
all: strtopid

strtopid: strtopid.h

.PHONY: tests
tests: SHELL = /bin/bash
tests: export override PATH := .:$(PATH)
tests: strtopid
	strtopid "1"
	strtopid "127"
	strtopid "32768"
	! strtopid "0"
	! strtopid "\-1"
	! strtopid "pid"
	! strtopid "pid1"

.PHONY: clean
clean:
	rm -f strtopid *.o

