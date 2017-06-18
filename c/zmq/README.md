# publisher(1), subscriber(1)

## NAME

**publisher** - simple ZMQ publisher

**epoll-publisher** - simple ZMQ publisher

**subscriber** - simple ZMQ subscriber

## SYNOPSIS

**publisher** [OPTIONS] ENDPOINT TOPIC MESSAGE

**epoll-publisher** [OPTIONS] ENDPOINT TOPIC [FILE=STDIN]

**subscriber** [OPTIONS] ENDPOINT TOPIC

## DESCRIPTION

**publisher(1)** binds to _ENDPOINT_ and publishes a _MESSAGE_ with _TOPIC_
every _1000 applet milli-seconds. The number of milli-seconds can be specified
using option _-t_.

**epoll-publisher(1)** binds to _ENDPOINT_ and publishes every line read from
_FILE_ to _TOPIC_.

**subscriber(1)** connects to _ENDPOINT_ and subscribes to _TOPIC_. Every
messages received are printed to _stdout_.

## OPTIONS

**-t or --timeout MS**
	Set time-out in seconds.

**-v or --verbose**
	Turn on verbose messages.

**-D or --debug**
	Turn on debug messages.

**-h or --help**
	Display this message.

**-V or --version**
	Display the version.

## EXAMPLES

Open two terminals, one runs:

	$ publisher "tcp://*:5555" "my-topic" "my message to send"
	my message to send
	my message to send
	my message to send
	...

Second runs:

	$ subscriber "tcp://*:5555" "my-topic"
	my message to send
	my message to send
	my message to send
	...

## AUTHOR

Written by Gaël PORTAY *gael.portay@gmail.com*

## COPYRIGHT

Copyright (c) 2015-2017 Gaël PORTAY

This program is free software: you can redistribute it and/or modify it under
the terms of the MIT License.
