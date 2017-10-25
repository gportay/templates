#
# Copyright (C) 2017 Savoir-Faire Linux Inc.
#
# Authors:
#       Gaël PORTAY <gael.portay@savoirfairelinux.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 2 of the License, or
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

override CFLAGS+=-Wall -Wextra -Werror

.PHONY: all
all: pipe-exec pipe

pipe-exec: pipe-exec.o pexec.o

.PHONY: test
test: pipe-exec
	./$< $$PWD/test.sh -vDD

.PHONY: clean
clean:
	rm -f pipe-exec *.o pipe

