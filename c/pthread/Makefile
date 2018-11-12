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
all: pthread-signal

pthread-signal: override CFLAGS += -pthread

.PHONY: run
tests run: export override PATH := .:$(PATH)
run:

.PHONY: run tests
run tests: export override PATH := .:$(PATH)

run: pthread-signal
	pthread-signal -v one two three

tests: pthread-signal
	pthread-signal -v one two three & sleep 1; kill -INT "$$!" && wait "$$!"
	pthread-signal -v one two three & sleep 1; kill -USR1 "$$!" && sleep 1 && kill -USR2 "$$!" && sleep 1 && kill "$$!" && wait "$$!"

.PHONY: clean
clean:
	rm -f pthread-signal

