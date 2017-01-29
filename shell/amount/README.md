# amount(1)

## NAME

[amount](amount) - serial-disk auto-mounter

## SYNOPSIS

**amount** [OPTIONS] DEVICE [MOUNTDIR=/media/<devname>] [-- MOUNT_OPTIONS]

## DESCRIPTION

Auto-mount serial-disk under **/media**.

If _MOUNTDIR_ is left unspecified, **amount(1)** uses _/media/LABEL_. It falls
back to _/media/DEVNAME_ if _DEVICE_ has no _LABEL_ or if _FS_ does not support
for _labels_.

If _DEVICE_ is already mounted, **amount(1)** exits success and reports a
warning onto _stderr_.

If _MOUNTDIR_ already exists, **amount(1)** suffixes it by *_num* where
_num_ is the next _mount-point_ available.

When _DEVICE_ is mounted **amount(1)** outputs the _mount-point_ onto _stdout_
and runs helpers under _/usr/share/amount.d/_ using _mount_ as first argument
and _DEVNAME_ as second.

## OPTIONS

**-h or --help**
	Display this message.

**-v or --version**
	Display the version.

## EXAMPLES

In its simpliest form, here is an example to mount device _/dev/sdb1_

	# amount /dev/sdb1
	/media/data

The device is mounted under its **LABEL** (i.e. _data_). In the case of the
_FS_ has no _label_, the _mount-point_ would have been its **DEVNAME** (i.e.
_sdb1_).

The _mount-point_ can be specified as second argument

	# amount /dev/sdb1 /mnt
	/mnt

Extra arguments can be passed to **mount(1)** after the _--_

	# amount /dev/sdb1 /mnt/data-ro -- -oro
	/mnt/data-ro

## SAMPLES FILES

[/etc/udev/rules.d/55-sd.rules](55-sd.rules) - udev rule for sd devices

[/usr/share/udev/sd.action](sd.action) - udev run script for sd devices

[/usr/share/amount.d/notify](notify.action) - notify helper

## AUTHOR

Written by Gaël PORTAY *gael.portay@gmail.com*

## COPYRIGHT

Copyright (c) 2016-2017 Gaël PORTAY

This program is free software: you can redistribute it and/or modify it under
the terms of the MIT License.
