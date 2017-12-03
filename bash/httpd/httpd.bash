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

declare -A request
declare -A headers

# Read request header line by line
while read -r line
do
	# Split into words and strip trailing CR
	words=( ${line%%$'\r'} )

	# End of request
	if [ "${#words[@]}" -eq 0 ]
	then
		break
	fi

	# Begin of request
	if [ "${#request[@]}" -eq 0 ]
	then
		request[METHOD]="${words[0]}"
		request[URI]="${words[1]}"
		request[VERSION]="${words[2]}"
		continue
	fi

	# Request header
	if [ "${#words[@]}" -eq 3 ]
	then
		words=( "${words[0]}" "${words[2]}" )
	else
		words[0]="${words[0]%:}"
	fi

	words[0]="${words[0]//-/_}"
	words[0]="${words[0]^^}"
	eval "headers[${words[0]}]=\"${words[1]}\""
done

# Extract path and query
uri="${request[URI]}"
query="${uri#*\?}"
path="${uri%\?*}"
uri="$path"
if [[ "$query" == "$path" ]]
then
	query=""
fi
if [[ "$path" =~  ^/cgi-bin/* ]]
then
	cgi=true
else
	cgi=false
fi
path="${DOCUMENT_ROOT:-$PWD}$path"
if [ -d "$path" ]
then
	if $cgi
	then
		path+="index.cgi"
	else
		path+="index.txt"
	fi
fi

# Export headers
for key in "${!headers[@]}"
do
	eval "export HTTP_$key=\"${headers[$key]}\""
done

# Debug
if [[ $DEBUG ]]
then
	echo "METHOD=\"${request[METHOD]}\""
	echo "URI=\"${request[URI]}\""
	echo "VERSION=\"${request[VERSION]}\""
	echo "PATH=$path"
	echo "QUERY=$query"
	for key in "${!headers[@]}"
	do
		echo "$key=\"${headers[$key]}\""
	done
	echo
fi >&2

# 404 Not Found
if [ ! -e "$path" ]
then
	printf "HTTP/1.1 404 Not Found\r\n"
	printf "\r\n"
	exit 1
fi

# WebSocket handshake
if [ "$HTTP_UPGRADE" = "websocket" ] &&
   [ "$HTTP_CONNECTION" = "Upgrade" ] &&
   [ -n "$HTTP_SEC_WEBSOCKET_KEY" ] &&
   [ "$HTTP_SEC_WEBSOCKET_VERSION" -ge "13" ]
then
	accept="$HTTP_SEC_WEBSOCKET_KEY"
	accept+="258EAFA5-E914-47DA-95CA-C5AB0DC85B11"
	accept="$(echo -n "$accept" | openssl dgst -sha1 -binary | base64)"
	printf "HTTP/1.1 101 Switching Protocols\r\n"
	printf "Upgrade: websocket\r\n"
	printf "Connection: Upgrade\r\n"
	printf "Sec-WebSocket-Accept: %s\r\n" "$accept"
	printf "Sec-WebSocket-Protocol: chat\r\n"
	printf "\r\n"
	exec "$path"
fi

# CGI script
if $cgi
then
	GATEWAY_INTERFACE="CGI/1.1"
	export GATEWAY_INTERFACE

	QUERY_STRING="$query"
	export QUERY_STRING

	REQUEST_METHOD="${request[METHOD]}"
	REQUEST_URI="${request[URI]}"
	REQUEST_VERSION="${request[VERSION]}"
	export REQUEST_METHOD REQUEST_URI REQUEST_VERSION

	SCRIPT_NAME="$uri"
	SCRIPT_FILENAME="$path"
	export SCRIPT_FILENAME SCRIPT_NAME

	exec "$path"
fi

# 200 OK
content_length="$(stat -c %s "$path")"
printf "HTTP/1.1 200 OK\r\n"
printf "Content-Length: %s\r\n" "$content_length"
printf "Content-type: text/plain\r\n";
printf "\r\n"
cat "$path"
