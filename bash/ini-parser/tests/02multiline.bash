#!/bin/bash

set -e

cleanup() {
	rc=$?
	if [ "$rc" -ne 0 ]; then
		IFS=$'\n'
		echo -e "\e[31;1m${ini[*]}\e[0m"
		exit 1
	fi
}

if [ $# -lt 1 ]; then
	echo "Usage: ${0##*/} PARSER" >&2
	echo "Error: Too few arguments!" >&2
	exit 1
fi

. $1

trap cleanup 0

ini_parser ${0/.bash/.ini}

ini_multiline

echo "checking for key_1: '$key_1' = 'This is a multiline value
 including spaces escaped'"
test "$key_1" = This\ is\ a\ multiline\ value\
\ including\ spaces\ escaped

echo "checking for key_2: '$key_2' = 'This is a multiline value including spaces quoted'"
test "$key_2" = 'This is a multiline value
 including spaces quoted'

echo "checking for key_3: '$key_3' = 'This is a multiline value including spaces double quoted'"
test "$key_3" = "This is a multiline value
 including spaces double quoted"
