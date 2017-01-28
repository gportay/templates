# ini-parser.bash(7)

## NAME

[ini-parser.bash](ini-parser.bash.7.adoc) - an ini-style parser written in bash
for bash

## DESCRIPTION

[ini-parser.bash](ini-parser.bash) reformats _ini section_ headers so they can
be interpreted by a _shell_.

## ALGORITHM

The algorithm of translation from _ini_ to _shell_ is both tricky and simple.

Every single _ini section_ is transformed into a _shell function_, and nothing
more!

The algorithm acts as follow.

When an ini *[section]* is met, the algorithm first closes the previous section
by adding a shell function close bracket _}_. Then it transforms the ini
*[section]* into a shell function _section() {_

	[section1]
	key1=val1
	key2=val2

	[section2]
	key3=val3

	}
	section1() {
	key1=val1
	key2=val2
	}
	section2() {
	key3=val3

Finally, one more trick to make the translated _shell_ valid. The algorithm
moves the very first close bracket _}_ at line _1_ to the _end_.

	section1() {
	key1=val1
	key2=val2
	}
	section2() {
	key3=val3
	}	

A last trick is needed to handle empty _sections_. _POSIX shells_ complain in
case of an empty function. This empty function generates an error.

	empty() {
	}


To avoid complains the trick consists in adding the _no-op_ colon instruction
_:_, followed by the semi-colon instruction separator _;_ before closing the
function.

	empty() {
	:;}

## LINKS

Check for the [man-page](ini-parser.bash.7.adoc),
[examples](ini-parser.bash.7.adoc#examples) and
[limitations](ini-parser.bash.7.adoc#limitations) sections,
[singleline.ini](tests/01singleline.ini),
[multiline.ini](tests/02multiline.ini) and
[variables.ini](tests/03variables.ini) ini files used for tests.

## REQUIREMENT

The algorithm needs *bash* as only requirement.

It uses _arrays_ to handle the file content line-by-line, and _replacement
substition_.

Since version _4.3_, bash has changes substitution behaviors and script needs
to set the flag _compat42_.

	If set, bash does not process the replacement string in the pattern
	substitution word expansion using quote removal.

## BUGS

Report bugs at *https://github.com/gazoo74/templates/issues*

## AUTHOR

Written by Gaël PORTAY *gael.portay@savoirfairelinux.com*

Widely inspired on work from [Andrés J. Díaz](http://theoldschooldevops.com/2008/02/09/bash-ini-parser/).

## COPYRIGHT

Copyright (c) 2017 Gaël PORTAY

This program is free software: you can redistribute it and/or modify it under
the terms of the MIT License.
