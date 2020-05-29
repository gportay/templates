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

.PHONY: clean
clean:
	rm -f input-eventd

