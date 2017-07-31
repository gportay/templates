# prompt_yes_no

## NAME

[prompt_yes_no](prompt) - prompt user for a yes/no reply

## SYNOPSIS

**prompt_yes_no** QUESTION [Yes/no]|[No/yes]

## DESCRIPTION

**prompt_yes_no** is a shell function that ask user for a _yes/no_ reply.

It returns _0_ if user answers _yes_ (or _y_) or _1_ if user answers _no_ (or
_n_).

**prompt_yes_no** is case independent; _Yes_, _YES_, _YeS_ are valid replies for
_yes_.

The reply is read from *stdin*; re-ask when the reply is invalid (i.e. reply is
neither _yes_ nor _no_).

## EXAMPLES

Here is a simple example of how to use _prompt\_yes\_no_. It handles _yes_ and
_no_ replies, whatever the case is used (_upper_ or _lower_).

	$ prompt_yes_no "Do you want to exit success" "[No/yes]"
	Do you want to exit success  [No/yes]? YES
	$ echo $?
	0

	$ prompt_yes_no "Do you want to exit success" "[No/yes]"
	Do you want to exit success  [No/yes]? no
	$ echo $?
	1

It re-asks when replies are invalid (_abc_, _def_...)

	$ prompt_yes_no "Do you want to exit success" "[No/yes]"
	Do you want to exit success  [No/yes]? abc
	Invalid response!
	Do you want to exit success  [No/yes]? def
	Invalid response!
	Do you want to exit success  [No/yes]?
	...

It returns the default option in case of empty replies; when _\<Enter\>_ is
entered

	$ prompt_yes_no "Do you want to exit success" "[No/yes]"
	Do you want to exit success  [No/yes]? <Enter>
	$ echo $?
	1

Or when *stdin* returns *EOF* (_\<Ctrl+D\>_)

	$ prompt_yes_no "Do you want to exit success" "[No/yes]"
	Do you want to exit success  [No/yes]? <Ctrl+D>
	$ echo $?
	1

Here is a simple example of how to use it in a _shell_ script

	if prompt_yes_no "Are you sure you want to remove directory" "[No/yes]"
	then
		rm -rf directory-ask-to-be-removed/
	fi

## AUTHOR

Written by Gaël PORTAY *gael.portay@gmail.com*

## COPYRIGHT

Copyright (c) 2015-2017 Gaël PORTAY

This program is free software: you can redistribute it and/or modify it under
the terms of the MIT License.
