#!/bin/bash
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

set -e

run() {
	id=$((id+1))
	test="#$id: $@"
	echo -e "\e[1mRunning $test...\e[0m"
}

ok() {
	ok=$((ok+1))
	echo -e "\e[1m$test: \e[32m[OK]\e[0m"
}

ko() {
	ko=$((ko+1))
	echo -e "\e[1m$test: \e[31m[KO]\e[0m"
}

result() {
	if [ -n "$ok" ]; then
		echo -e "\e[1m\e[32m$ok test(s) succeed!\e[0m"
	fi

	if [ -n "$ko" ]; then
		echo -e "\e[1mError: \e[31m$ko test(s) failed!\e[0m" >&2
		exit 1
	fi
}

trap result 0

HOST="localhost"
PORT="1234"
BASEURI="http://$HOST:$PORT"
SERVER="httpd.bash"
CURLOPTS=( $CURLOPTS )
CURLEXTRAOPTS=( )

DOCUMENT_ROOT="$PWD"
export DOCUMENT_ROOT

SERVER_ADMIN="$USER@$HOST"
SERVER_NAME="$HOST"
SERVER_PORT="$PORT"
SERVER_SOFTWARE="$SERVER"
export SERVER_ADMIN SERVER_NAME SERVER_PORT SERVER_SOFTWARE

send_request() {
	nc -l "$HOST" "$PORT" --sh-exec "bash $SERVER" &
	trap "kill $!; wait $!; result" 0 INT

	run "Test $uri"
	if curl "$@" "${CURLEXTRAOPTS[@]}" "${CURLOPTS[@]}"
	then
		ok
	else
		ko
	fi
	echo

	trap result 0
}

uri="$BASEURI"
send_request "$uri"

uri+="?foo=bar"
send_request "$uri"

uri="$BASEURI/index.txt"
send_request "$uri"

uri="$BASEURI"
uri+="?foo=bar"
send_request "$uri"

uri="$BASEURI"
uri+="/export.sh"
send_request "$uri"

uri+="?foo=bar"
send_request "$uri"

uri="$BASEURI"
uri+="/cgi-bin/export.sh"
send_request "$uri"

uri+="?foo=bar"
send_request "$uri"

CURLEXTRAOPTS+=( --header "Connection: Upgrade" )
CURLEXTRAOPTS+=( --header "Upgrade: websocket" )
CURLEXTRAOPTS+=( --header "Host: $HOST:$PORT" )
CURLEXTRAOPTS+=( --header "Origin: http://$HOST:$PORT" )
CURLEXTRAOPTS+=( --header "Sec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==" )
CURLEXTRAOPTS+=( --header "Sec-WebSocket-Version: 13" )

uri="$BASEURI"
uri+="/websocket.sh"
send_request "$uri"

uri+="?foo=bar"
send_request "$uri"
