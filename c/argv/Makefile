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
all: strtonargv

strtonargv: strtonargv.h

.PHONY: tests
tests: SHELL = /bin/bash
tests: export override PATH := .:$(PATH)
tests: strtonargv
	CFS=";" strtonargv "/usr/bin/dbus-daemon;--system;--address=systemd:;--nofork;--nopidfile;--systemd-activation;--syslog-only;" | \
	diff - <(printf "/usr/bin/dbus-daemon --system --address=systemd: --nofork --nopidfile --systemd-activation --syslog-only\n")
	CFS=";" strtonargv --count "/usr/bin/dbus-daemon;--system;--address=systemd:;--nofork;--nopidfile;--systemd-activation;--syslog-only;" | \
	diff - <(echo "7")
	IFS=$$'\n' strtonargv "strtonargv(1) splits strings to a list of words." | \
	diff - <(printf "strtonargv(1)\nsplits\nstrings\nto\na\nlist\nof\nwords.\n")
	IFS=$$'\n' strtonargv --count "strtonargv(1) splits strings to a list of words." | \
	diff - <(echo "8")
	CFS=$$'\n' strtonargv "strtonargv(1) splits strings to a list of words." | \
	diff - <(printf "strtonargv(1) splits strings to a list of words.\n")
	CFS=$$'\n' strtonargv --count "strtonargv(1) splits strings to a list of words." | \
	diff - <(echo "1")

.PHONY: clean
clean:
	rm -f strtonargv *.o

