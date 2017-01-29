# Makefile

## NAME

[Makefile](Makefile) - build a repository of archive using foreach
function

## SYNOPSIS

$(**foreach** _var_,_list_,_text_)

_var_ is the declared iterate variable. It _should_ be used in _text_
expression.

_list_ is a word separated list. _var_ takes every values of this list.

_text_ is the expression that is evaluated with _var_.

## DESCRIPTION

This _Makefile_ is an example of how to use
[foreach](https://www.gnu.org/software/make/manual/html_node/Foreach-Function.html)
function. It is used conjunction with
[define](https://www.gnu.org/software/make/manual/html_node/Multi_002dLine.html)
directive,
[eval](https://www.gnu.org/software/make/manual/html_node/Eval-Function.html)
and
[call](https://www.gnu.org/software/make/manual/html_node/Call-Function.html)
functions.

	all:
	@$(foreach i,one two three,echo $(i);)

	one two three

To make a parallel with _shell_

	for i in one two three; do
		echo $i;
	done

	one two three

## EXAMPLES

The [Makefile](Makefile#L10**) contains its own example

	pkg-m		:= a b c
	a-vers		:= 0 1 2
	a-deps		:= b c
	a-preinst	:= true
	b-vers		:= 3 4 5
	b-deps		:= c
	c-vers		:= 0
	c-postinst	:= false

Here, it builds packages _a-0_, _a-1_, _a-2_, _b-3_, _b-4_, _b-5_ and _c-0_.
Package _a_ has a _preinst_ script and _c_ a _postinst_ script.

**pkg-m** is the top level variable. It lists every packages to build (i.e.
_a_, _b_ and _c_).

Every packages can specify its _versions_, its _dependencies_ and if it has
package _scripts_. **\<pkgname\>-vers** lists every versions. If unspecified,
_1_ is assumed. **\<pkgname\>-deps** lists all dependencies by _\<pkgname>\_.
Both variables are space separated list. **\<pkgname\>-preinst**,
**\<pkgname\>-postinst**, **\<pkgname\>-prerm** and **\<pkgname\>-postrm** are
boolean (_true_ of _false_) to tells if _\<pkgname\>_ has one of this package
script.

The Makefile defines **do\_pkg($pkg)** at top level for each **_tgz-m**
packages. Then, this define-directive calls **do\_pkg\_control($pkg, $vers)**
for each versions of _<pkgname>-vers_ to generates the debian style control
file. Finally, this define calls **do_pkg_script($pkg, $vers, $script)** for
every boolean _<pkgname>-<pkgscript>_ which is set to true.

The following _shell_ script is a good candidate to understand what the
Makefile is doing

	for pkg in ...; do
		do_pkg $pkg
	done

	do_pkg() {
		for vers in ...; do
			do_pkg_control $1 $vers
		done
	}

	do_pkg_control() {
		...
		for script in preinst postinst prerm postrm; do
			do_pkg_script $1 $2 $script
		done
	}

	do_pkg_script() {
		...
	}

## AUTHOR

Written by Gaël PORTAY *gael.portay@gmail.com*

## COPYRIGHT

Copyright (c) 2016-2017 Gaël PORTAY

This program is free software: you can redistribute it and/or modify it under
the terms of the MIT License.
