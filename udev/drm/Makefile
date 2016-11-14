#
# Copyright 2016 Gaël PORTAY <gael.portay@gmail.com>
#
# Licensed under the MIT license.
#

PREFIX	?= /usr/local
DPDEVS	?= HDMI2 DP2
#DPPOS	?= right

.PHONY: all
all: 52-drm.rules
	@echo "Please adapt the \`$(CURDIR)/hotplug-monitor' script according to your setup." >&2
	@echo "Once ready, you may run:" >&2
	@echo "$$ make install [PREFIX=$(PREFIX)] [DPDEVS=$(DPDEVS)] [DPPOS=$(DPPOS)]'" >&2
	@echo "to install udev rule(s) and script onto your system." >&2
	@echo "" >&2
	@echo "Here is an example where the extra monitor is connected through the second" >&2
	@echo "display-port plug (ie. DP2) of the laptop and the monitor is placed on the" >&2
	@echo "left side of the laptop screen (LVDS1)." >&2
	@echo "$$ sudo make install DPDEV=DP2 DPPOS=left" >&2

52-drm.rules.in: 52-drm.rules
%: %.in
	@sed -e '/SUBSYSTEM=="drm"/s#$$#, ENV{DISPLAY}="$(DISPLAY)"#' \
	     -e '/SUBSYSTEM=="drm"/s#$$#, ENV{XAUTHORITY}="$(XAUTHORITY)"#' \
	       $< >$@

.PHONY: install
install:
	install -m 0755 -d $(DESTDIR)/etc/udev/rules.d/
	install -m 0644 52-drm.rules $(DESTDIR)/etc/udev/rules.d/
	sed -e 's,/usr/local/,$(PREFIX)/,' \
	    -i $(DESTDIR)/etc/udev/rules.d/52-drm.rules
	install -m 0755 -d $(DESTDIR)$(PREFIX)/bin/
	install -m 0755 hotplug-monitor $(DESTDIR)$(PREFIX)/bin/
	sed -e 's,/usr/local/,$(PREFIX)/,' \
	    -e '/^DPDEV/s,=.*,="$(DPDEV)",' \
	    -e '/^DPPOS/s,=.*,="$(DPPOS)",' \
	    -i $(DESTDIR)$(PREFIX)/bin/hotplug-monitor
	install -m 0755 -d $(DESTDIR)$(PREFIX)/share/hotplug-monitor.d/
	install -m 0755 i3 $(DESTDIR)$(PREFIX)/share/hotplug-monitor.d/90i3

.PHONY: udev-reload
udev-reload:
	udevadm control --reload-rules

.PHONY: monitor
udev-monitor:
	udevadm monitor --property

.PHONY: udev-test
udev-test:
	devpath="$$(udevadm info -q path -n /dev/dri/card0)"; \
	udevadm test --action=change "$$devpath"

uevent-add uevent-remove uevent-change:
uevent-%:
	@devpath="$$(udevadm info -q path -n /dev/dri/card0)"; \
	echo "$*" >"/sys/$$devpath/uevent"

.PHONY: test
test:
	@echo "Switch off $(DPDEV) and wait 3 seconds..."
	xrandr --output "$(DPDEV)" --off && sleep 3
	@echo "Simulate connection of $(DPDEV)..."
	sudo $(MAKE) -f Makefile uevent-change

.PHONY: i3
i3:
	@if grep '^bindsym $$mod+Mod1+Control+m' ~/.config/i3/config; then \
		echo "Error: Binding \$$mod+Mod1+Control+m' is already taken! ^"; \
		exit 1; \
	fi
	echo 'bindsym $$mod+Mod1+Control+m exec "$(PREFIX)/hotplug-monitor"' >>~/.config/i3/config

.PHONY: clean
clean:
	rm -f 52-drm.rules

