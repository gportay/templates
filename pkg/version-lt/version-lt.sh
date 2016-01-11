#!/bin/sh
#
# Copyright 2016 Gaël PORTAY <gael.portay@gmail.com>
#
# Licensed under the MIT license.
#

VERSION=1.0

die() {
	echo "Error: $*!" >&2
	exit 1
}

usage() {
	cat <<EOF
Usage: ${0##*/} VERS VERS_REF

Prints the Lower version of VERS and VERS_REF.

Exits success if VERS is Lower Than (lt) VERS_REF.

Options:
 -e or --equal         Lower or Equal.
 -q or --quiet         Quiet.
 -h or --help          Display this message.
 -V or --version       Display the version.
EOF
}

parse_arguments() {
	vers=
	vers_ref=
	while [ $# -ne 0 ]; do
		if [ "$1" = "-e" ] || [ "$1" = "--equal" ]; then
			EQUAL=true
		elif [ "$1" = "-q" ] || [ "$1" = "--quiet" ]; then
			QUIET=q
		elif [ "$1" = "-h" ] || [ "$1" = "--help" ]; then
			usage
			exit 0
		elif [ "$1" = "-V" ] || [ "$1" = "--version" ]; then
			echo $VERSION
			exit 0
		elif [ -z "$vers" ]; then
			vers=$1
		elif [ -z "$vers_ref" ]; then
			vers_ref=$1
		else
			usage
			die "$1: Too many arguments!"
		fi
		shift
	done
}

version_lt() {
	if which verrevcmp.awk >/dev/null 2>&1; then
		if echo "$1" "$2" | verrevcmp.awk; then
			echo $1
		else
			echo $2
		fi
	else
		cat <<EOF | sort -V | head -n1
$1
$2
EOF
	fi | grep -v$QUIET "$2"
}

parse_arguments $*

if [ -z "$vers" ] || [ -z "$vers_ref" ]; then
	usage
	die "Too few arguments!"
fi

version_lt "$vers" "$vers_ref" || ( ${EQUAL:-false} && echo "$vers" | grep -E$QUIET "^$vers_ref$" )
