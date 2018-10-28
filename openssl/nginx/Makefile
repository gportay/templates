#
#  Copyright (C) 2018 Gaël PORTAY
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

DESTDIR ?=

C  ?= CA
ST ?= Quebec
L  ?= Montreal
O  ?= L'electron du libre
OU ?= dev

.PHONY: all
all: ca.crt

newcerts:
	mkdir $@

serial:
	echo "00" >$@

index.txt:
	touch $@

ca.key:
	openssl genrsa -out $@ 2048

ca.crt: CN ?= www.portay.io
ca.crt: ca.key
	openssl req -new -x509 -days 3650 \
		    -subj "/C=$(C)/ST=$(ST)/L=$(L)/O=$(O)/OU=$(OU)/CN=$(CN)" \
		    -key $< -out $@

openssl.cnf: | newcerts index.txt serial
	sed -e '/^dir/adir		= $$ENV::PWD' \
	    /etc/ssl/openssl.cnf >$@

.PHONY: install
install: SHELL=/bin/bash
install:
	install -d $(DESTDIR)/etc/ssl/
	cp -a newcerts $(DESTDIR)/etc/ssl/
	sed -e '/^dir		= $$ENV::PWD/d' openssl.cnf >$(DESTDIR)/etc/ssl/openssl.cnf
	install index.txt{,.attr} serial $(DESTDIR)/etc/ssl/

.PHONY: clean
clean:
	rm -rf openssl.cnf newcerts index.txt serial *.key *.crt

