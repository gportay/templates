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

ini_scalar

echo "checking for scalar: '$scalar' = 'one'"
test "$scalar" = 'one'

echo "checking for key_1: '$key_1' = 'one'"
test "$key_1" = "one"

ini_array

echo "checking for array: '${array[*]}' = 'one two three'"
test "${array[0]}" = "one"
test "${array[1]}" = "two"
test "${array[2]}" = "three"

echo "checking for key_0: '$key_0' = 'one two three'"
test "$key_0" = "one two three"

echo "checking for key_1: '$key_1' = 'one'"
test "$key_1" = "one"

echo "checking for key_2: '$key_2' = 'two'"
test "$key_2" = "two"

echo "checking for key_3: '$key_3' = 'three'"
test "$key_3" = "three"
