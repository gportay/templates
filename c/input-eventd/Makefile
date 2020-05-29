#
# Copyright 2020 Gaël PORTAY
#
# SPDX-License-Identifier: BSD 4-Clause
#

.PHONY: all
all: input-eventd

define do_test
test-$(1)-$(2): PATH:=$(CURDIR):$(PATH)
test-$(1)-$(2): input-eventd
	input-eventd --verbose --script $(2).script /dev/input/$(1)
endef

define do_test_script
$(foreach dev,$(subst /dev/input/,,$(wildcard /dev/input/event*)),$(eval $(call do_test,$(dev),$(1))))
endef

$(foreach script,$(subst .script,,$(wildcard *.script)),$(eval $(call do_test_script,$(script))))

.SILENT: input-event-codes.bash
.ONESHELL: input-event-codes.bash
input-event-codes.bash: SHELL=/bin/bash
input-event-codes.bash: PATH:=$(CURDIR):$(PATH)
input-event-codes.bash: /usr/include/linux/input-event-codes.h
	if test -e /usr/include/linux/input-event-codes.h
	then
		echo "# SPDX-License-Identifier: GPL-2.0-only WITH Linux-syscall-note"
		echo "#"
		echo "# Automatically generated file; DO NOT EDIT."
		echo "#"
		eval "$$(sed -n '/^#define[[:blank:]]\+KEY_/s,#define[[:blank:]]\+\(KEY_[[:alnum:]]\+\)[[:blank:]]\+\([[:alnum:]]\+\).*$$,KEYS[$$((\2))]=\1,p' $<)"
		for key in "$${!KEYS[@]}"
		do
			echo "KEYS[$$key]=$${KEYS[$$key]}"
		done
	fi >$@

.PHONY: clean
clean:
	rm -f input-eventd input-event-codes.bash

