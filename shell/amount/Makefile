#
# Copyright 2016 Gaël PORTAY <gael.portay@gmail.com>
#
# Licensed under the MIT license.
#

PREFIX	?= /usr/local

# The USER is required to mount the sd device with the appropriate owership.
# udev triggers device (un)mount but it does not know which USER is currently
# logged-in.
#
# As a dirty workaround, this Makefile assumes your are the only USER of this
# computer and as a consequence, the only USER allowed to mount sd devices.
#
# Makefile checks for the USER= variable given through the command-line in the
# first place. Then it looks for environments variables $SUDO_USER or $USER.
# Finally, it ends with the whoami command if nothing has been set.
#
# Examples:
# myself@localhost$ make USER=someone-else
# Will set amount to mount sd device with someone-else owner.
#
# myself@localhost$ sudo make USER=someone-else
# Will also set amount to mount sd device with someone-else ownership.
#
# myself@localhost$ make
# Will set amount to mount sd device with myself ownership.
#
# myself@localhost$ make
# Will also set amount to mount sd device with myself ownership.
#
# myself@localhost$ make USER=
# Will also set amount to mount sd device with myself ownership.
#
# myself@localhost$ sudo make USER=
# Will set amount to mount sd device with root ownership.
#
# Check if USER= is given through the command-line;
ifeq ("$(origin USER)", "command line")
	user := $(USER)
else
# otherwise, check is either $SUDO_USER...
ifneq (,$(SUDO_USER))
	user := $(SUDO_USER)
# ... or $USER environments are set;
else
	user := $(USER)
endif
endif

# finally, check for whoami command to get the user-name.
ifeq (,$(user))
	user := $(shell whoami)
endif

.PHONY: all
all: help

.PHONY: help
help:
	@echo "You may run:" >&2
	@echo "$$ make install [PREFIX=$(PREFIX)] [USER=$(user)]'" >&2
	@echo "to install udev rule(s) and script(s) onto your system." >&2

.PHONY: install
install: 55-sd.rules
	install -m 0755 -d $(DESTDIR)/etc/udev/rules.d/
	install -m 0644 55-sd.rules $(DESTDIR)/etc/udev/rules.d/
	sed -e 's,/usr/local/,$(PREFIX)/,' \
	    -e '/KERNEL==/s#$$#, ENV{USER}="$(user)"#' \
	    -i $(DESTDIR)/etc/udev/rules.d/55-sd.rules
	install -m 0755 -d $(DESTDIR)$(PREFIX)/share/udev/
	install -m 0755 sd.action $(DESTDIR)$(PREFIX)/share/udev/
	sed -e 's,/usr/local/,$(PREFIX)/,' \
	    -i $(DESTDIR)$(PREFIX)/share/udev/sd.action
	install -m 0755 -d $(DESTDIR)$(PREFIX)/bin/
	install -m 0755 amount $(DESTDIR)$(PREFIX)/bin/
	sed -e 's,/usr/local/,$(PREFIX)/,' \
	    -i $(DESTDIR)$(PREFIX)/bin/amount
	install -m 0755 -d $(DESTDIR)$(PREFIX)/share/amount.d/
	install -m 0755 notify.action $(DESTDIR)$(PREFIX)/share/amount.d/50notify
	install -m 0755 i3blocks $(DESTDIR)$(PREFIX)/share/amount.d/90i3blocks

install-i3: install
	install -m 0755 -d $(DESTDIR)$(PREFIX)/share/amount.d/
	install -m 0755 i3blocks $(DESTDIR)$(PREFIX)/share/amount.d/90i3blocks
	install -m 0755 -d $(DESTDIR)$(PREFIX)/libexec/i3blocks/
	install -m 0755 serial_disk $(DESTDIR)$(PREFIX)/libexec/i3blocks/
	@if [ -z "$(DESTDIR)" ]; then \
		sed -e '/# Serial-Disks/,/^$$/d' \
		    -i $(HOME)/.config/i3blocks/config; \
		cat i3blocks.config >>$(HOME)/.config/i3blocks/config; \
	else \
		echo 'cat <<EOF >/home/$(user)/.config/i3blocks/config' ; \
		cat i3blocks.config; \
		echo 'EOF'; \
	fi

.PHONY: udev-reload
udev-reload:
	udevadm control --reload-rules

.PHONY: monitor
udev-monitor:
	udevadm monitor --property

.PHONY: udev-test
udev-test:
	devpath="$$(udevadm info -q path -n /dev/sdb1)"; \
	udevadm test --action=add "$$devpath"

uevent-add uevent-remove uevent-change:
uevent-%:
	@devpath="$$(udevadm info -q path -n /dev/sdb1)"; \
	echo "$*" >"/sys/$$devpath/uevent"

.PHONY: test
test:
	@echo "Simulate de-connection of /dev/sdb1 and wait 3 seconds..."
	$(MAKE) -f Makefile uevent-remove && sleep 3
	@echo "Simulate connection of /dev/sdb1..."
	$(MAKE) -f Makefile uevent-add


