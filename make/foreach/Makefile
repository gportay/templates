#!/usr/bin/make -f
#
# Copyright 2016-2017 Gaël PORTAY <gael.portay@gmail.com>
#
# Licensed under the MIT license.
#

PREFIX	?= /var/lib/pkg

pkg-m		:= a b c
a-vers		:= 0 1 2
a-deps		:= b c
a-preinst	:= true
b-vers		:= 3 4 5
b-deps		:= c
c-vers		:= 0
c-postinst	:= false

.PHONY: all

all::

.SILENT::

define do_pkg_script =
ifneq (,$($(1)-$(3)))
.SILENT:: tgz/$(1)-$(2)$(PREFIX)/info/$(1)/$(3)
tgz/$(1)-$(2)$(PREFIX)/info/$(1)/$(3):
	install -d $$(@D)
	echo "#!/bin/sh" >$$@
	echo "$$($(1)-$(3))" >>$$@
	chmod a+x $$@
	echo "$(3): $$($(1)-$(3))" | sed 's/^.\| [a-z]/XXX-\U&/'

$(1)-$(2)-script-y := tgz/$(1)-$(2)$(PREFIX)/info/$(1)/$(3)
endif
endef

define do_pkg_control =
.SILENT:: tgz/$(1)-$(2)$(PREFIX)/info/$(1)/control
tgz/$(1)-$(2)$(PREFIX)/info/$(1)/control:
	install -d $$(@D)
	echo "Package: $(1)" >$$@
	echo "Version: $(2)" >>$$@
	[ -z "$$($(1)-deps)" ] || echo "Depends: $$($(1)-deps)" >>$$@
	echo
	cat $$@

$(foreach script,preinst postinst prerm postrm,$(eval $(call do_pkg_script,$(1),$(2),$(script))))

pkgfiles-m += tgz/$(1)-$(2)$(PREFIX)/info/$(1)/control $($(1)-$(2)-script-y)
pkgdirs-m  += tgz/$(1)-$(2)

tgz/$(1)-$(2).tgz: tgz/$(1)-$(2)$(PREFIX)/info/$(1)/control $($(1)-$(2)-script-y)
endef

define do_pkg =
$(foreach vers,$(if $($(1)-vers),$($(1)-vers),1),$(eval $(call do_pkg_control,$(1),$(vers))))
endef

$(foreach pkg,$(pkg-m),$(eval $(call do_pkg,$(pkg))))

tgz-m := $(patsubst %,%.tgz,$(pkgdirs-m))

%.tgz:
	( cd $* && tar czf ../$(@F) * )

tgz/Index: $(tgz-m)
	install -d $(@D)/
	for tgz in $?; do \
		tar xOzf $$tgz $(PREFIX:/%=%)/info/*/control; \
		echo Filename: $${tgz##$(@D)/}; \
		echo; \
	done >$@
	cat $@

all:: tgz/Index

clean:
	rm -rf tgz/

