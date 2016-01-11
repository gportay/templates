#!/usr/bin/gmake -f
#
# Copyright 2016 Gaël PORTAY <gael.portay@gmail.com>
#
# Licensed under the MIT license.
#

PREFIX	?= /var/lib/pkg

pkg-m		:= a b c
a-vers		:= 0 1 2
a-deps		:= b c
b-vers		:= 3 4 5
b-deps		:= c
c-vers		:= 0

.PHONY: all

all::

.SILENT::

define pkg_control =
.SILENT:: tgz/$(1)-$(2)$(PREFIX)/$(1)/control
tgz/$(1)-$(2)$(PREFIX)/$(1)/control:
	install -d $$(@D)
	echo "Package: $(1)" >$$@
	echo "Version: $(2)" >>$$@
	[ -z "$$($(1)-deps)" ] || echo "Depends: $$($(1)-deps)" >>$$@
	cat $$@
	echo

pkgfiles-m += tgz/$(1)-$(2)$(PREFIX)/$(1)/control
endef

$(foreach pkg,$(pkg-m),$(eval $(call $(foreach vers,$($(pkg)-vers),$(eval $(call pkg_control,$(pkg),$(vers)))))))

all:: $(pkgfiles-m)

clean:
	rm -rf tgz/

