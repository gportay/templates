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

ini_singleline

echo "checking for key_0: '$key_0' = 'value'"
test "$key_0" = 'value'

echo "checking for key_1: '$key_1' = 'This is a value including spaces espaced'"
test "$key_1" = This\ is\ a\ value\ including\ spaces\ espaced

echo "checking for key_2: '$key_2' = 'This is a value including spaces quoted'"
test "$key_2" = 'This is a value including spaces quoted'

echo "checking for key_3: '$key_3' = 'This is a value including spaces qouble-quoted'"
test "$key_3" = "This is a value including spaces qouble-quoted"
