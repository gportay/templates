# zpc-publisher(1), zpc-subscriber(1)

## NAME

**zpc-publisher** - simple ZMQ publisher

**zpc-subscriber** - simple ZMQ subscriber

## SYNOPSIS

**zpc-publisher** [OPTIONS] ENDPOINT TOPIC

**zpc-subscriber** [OPTIONS] ENDPOINT TOPIC

## DESCRIPTION

**zpc-publisher(1)** binds to _ENDPOINT_ and publishes every line read from
stdin to _TOPIC_.

**zpc-subscriber(1)** connect to _ENDPOINT_ and subscribes to _TOPIC_.

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

	$ zpc-publisher "tcp://*:5555" "my-topic"

Second runs:

	$ zpc-subscriber "tcp://localhost:5555" "my-topic"

Every line entered in publisher is displayed to subscriber.

Uses option _-D_ to display protobuf overhead.

## AUTHOR

Written by Gaël PORTAY *gael.portay@gmail.com*

## COPYRIGHT

Copyright (c) 2015-2017 Gaël PORTAY

This program is free software: you can redistribute it and/or modify it under
the terms of the MIT License.
