#!/bin/bash
#
# Copyright (c) 2017 Gaël PORTAY <gael.portay@savoirfairelinux.com>
#
# This program is free software: you can redistribute it and/or modify
# the terms of the GNU General Public License as published by
# the Free Software Foundation, version 3.
#

run() {
	test="$@"
	echo -e "\e[1mRunning $test...\e[0m"
}

ok() {
	ok="$((ok+1))"
	echo -e "\e[1m$test: \e[32m[OK]\e[0m"
}

ko() {
	ko="$((ko+1))"
	echo -e "\e[1m$test: \e[31m[KO]\e[0m"
}

fix() {
	fix="$((fix+1))"
	echo -e "\e[1m$test: \e[34m[FIX]\e[0m"
}

bug() {
	bug="$((bug+1))"
	echo -e "\e[1m$test: \e[33m[BUG]\e[0m"
}

result() {
	if [ -n "$ok" ]; then
		echo -e "\e[1m\e[32m$ok test(s) succeed!\e[0m"
	fi

	if [ -n "$fix" ]; then
		echo -e "\e[1m\e[33m$fix test(s) fixed!\e[0m" >&2
	fi

	if [ -n "$bug" ]; then
		echo -e "\e[1mWarning: \e[33m$bug test(s) bug!\e[0m" >&2
	fi

	if [ -n "$ko" ]; then
		echo -e "\e[1mError: \e[31m$ko test(s) failed!\e[0m" >&2
		exit 1
	fi
}

. prompt

PATH="$PWD:$PATH"
trap result 0

response="Yes"
run "Test \`$response'"
if echo "$response" | \
   prompt_yes_no "Do you want to exit success" "[No/yes]" >/dev/null
then
	ok
else
	ko
fi
echo

run "Test \`${response^^}'"
if echo "${response^^}" | \
   prompt_yes_no "Do you want to exit success" "[No/yes]" >/dev/null
then
	ok
else
	ko
fi
echo

run "Test \`${response,,}'"
if echo "${response,,}" | \
   prompt_yes_no "Do you want to exit success" "[No/yes]" >/dev/null
then
	ok
else
	ko
fi
echo

response="y"
run "Test \`$response'"
if echo "$response" | \
   prompt_yes_no "Do you want to exit success" "[No/yes]" >/dev/null
then
	ok
else
	ko
fi
echo

run "Test \`${response^^}'"
if echo "${response^^}" | \
   prompt_yes_no "Do you want to exit success" "[No/yes]" >/dev/null
then
	ok
else
	ko
fi
echo

run "Test \`${response,,}'"
if echo "${response,,}" | \
   prompt_yes_no "Do you want to exit success" "[No/yes]" >/dev/null
then
	ok
else
	ko
fi
echo

response="No"
run "Test \`$response'"
if ! echo "$response" | \
     prompt_yes_no "Do you want to exit success" "[No/yes]" >/dev/null
then
	ok
else
	ko
fi
echo

run "Test \`${response^^}'"
if ! echo "${response^^}" | \
     prompt_yes_no "Do you want to exit success" "[No/yes]" >/dev/null
then
	ok
else
	ko
fi
echo

run "Test \`${response,,}'"
if ! echo "${response,,}" | \
     prompt_yes_no "Do you want to exit success" "[No/yes]" >/dev/null
then
	ok
else
	ko
fi
echo

response="n"
run "Test \`$response'"
if ! echo "$response" | \
     prompt_yes_no "Do you want to exit success" "[No/yes]" >/dev/null
then
	ok
else
	ko
fi
echo

run "Test \`${response^^}'"
if ! echo "${response^^}" | \
     prompt_yes_no "Do you want to exit success" "[No/yes]" >/dev/null
then
	ok
else
	ko
fi
echo

run "Test \`${response,,}'"
if ! echo "${response,,}" | \
     prompt_yes_no "Do you want to exit success" "[No/yes]" >/dev/null
then
	ok
else
	ko
fi
echo

response="abc"
run "Test \`${response:-(empty)}'"
if ! echo "$response" | \
     prompt_yes_no "Do you want to exit success" "[No/yes]" >/dev/null
then
	ok
else
	ko
fi
echo

response=""
run "Test \`${response:-(empty)}'"
if ! echo "$response" | \
     prompt_yes_no "Do you want to exit success" "[No/yes]" >/dev/null
then
	ok
else
	ko
fi
echo

response="abc"
run "Test \`${response:-(empty)}'"
if echo "$response" | \
   prompt_yes_no "Do you want to exit success" "[Yes/no]" >/dev/null
then
	ok
else
	ko
fi
echo

response=""
run "Test \`${response:-(empty)}'"
if echo "$response" | \
   prompt_yes_no "Do you want to exit success" "[Yes/no]" >/dev/null
then
	ok
else
	ko
fi
echo

