#
#  Copyright (C) 2017 Savoir-Faire Linux Inc.
#
#  Authors:
#      Gaël PORTAY <gael.portay@savoirfairelinux.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 2.1 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

.PHONY: all
all: tests

.PHONY: run
run: httpd

.PHONY: send-request
send-request: export DEBUG=1
send-request:
	curl http://localhost:1234 $(CURLOPTS)

.PHONY: httpd
httpd: export DEBUG=1
httpd:
	nc -l localhost 1234 --sh-exec "bash httpd.bash" $(NCOPTS)

.PHONY: ssl-send-request
ssl-send-request: export DEBUG=1
ssl-send-request: export CURLOPTS+=--insecure
ssl-send-request:
	curl https://localhost:1234 $(CURLOPTS)

.PHONY: ssl-httpd
ssl-httpd: export DEBUG=1
ssl-httpd: override NCOPTS+=--ssl
ssl-httpd:
	nc -l localhost 1234 --sh-exec "bash httpd.bash" $(NCOPTS)

.PHONY: openssl-httpd
openssl-server: server.pem
openssl-server: override OPENSSLOPTS+=-quiet
openssl-server:
	openssl s_server -accept 1234 $(OPENSSLOPTS)

.PHONY: openssl-client
openssl-client: server.pem
openssl-client: override OPENSSLOPTS+=-quiet
openssl-client:
	openssl s_client -connect localhost:1234 $(OPENSSLOPTS)

%.pem: OPENSSLOPTS=-subj "/C=CA/ST=Quebec/L=Montreal/O=DemoCA Ltd/OU=IT/CN=www.example.org"
%.pem:
	openssl req -newkey rsa:2048 \
	            -x509 -keyout $@ -out $@ -days 365 -nodes \
		    $(OPENSSLOPTS)

.PHONY: Sec-WebSocket-Accept
.SILENT: Sec-WebSocket-Accept
Sec-WebSocket-Accept: KEY?=x3JJHMbDL1EzLkh9GBhXDw==
Sec-WebSocket-Accept:
	echo -n "$(KEY)258EAFA5-E914-47DA-95CA-C5AB0DC85B11" | \
	openssl dgst -sha1 -binary | base64

.PHONY: tests
tests:
	bash tests.bash

.PHONY: ssl-tests
ssl-tests: export DEBUG=1
ssl-tests: export CURLOPTS+=--verbose --dump-header /dev/stderr
ssl-tests:
	bash tests.bash

.PHONY: debug-tests
debug-tests: export DEBUG=1
debug-tests: export CURLOPTS=--verbose --dump-header /dev/stderr
debug-tests:
	bash tests.bash

.PHONY: check
check: httpd.bash
	shellcheck $^

