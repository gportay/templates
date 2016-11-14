#
# Copyright 2016 Gaël PORTAY <gael.portay@gmail.com>
#
# Licensed under the MIT license.
#

user	?= $(SUDO_USER)
user	?= $(USER)
user	?= $(shell whoami)
group	?= android
RULES	:= $(wildcard *.rules)
RULES	:= $(filter-out 51-android.rules,$(RULES))

.PHONY: all
all: 51-android.rules

51-android.rules: $(RULES)
	wget -S -O - http://source.android.com/source/$@ | \
	sed -e "s/MODE=\"0600\"/MODE=\"0644\"/" \
	    -e "s/OWNER/GROUP/" \
	    -e "s/<username>/$(group)/" | cat - $? | \
	tee $@

.PHONY: setup
setup:
	groupadd --force --system $(group)
	usermod --append --groups $(group) $(user)

.PHONY: install
install: 51-android.rules
	install -m 0755 -d $(DESTDIR)/etc/udev/rules.d/
	install -m 0644 $< $(DESTDIR)/etc/udev/rules.d/

.PHONY: udev-reload
udev-reload:
	udevadm control --reload-rules

.PHONY: monitor
udev-monitor:
	udevadm monitor --property

adb_usb.ini: $(RULES)
	sed -n 's,.*ATTR{idVendor}=="\([0-9a-fA-F]*\)".*,0x\1,p' $^ | sort | uniq >$@

.PHONY: setup-adb
setup-adb: adb_usb.ini
	install -m 0644 $< $(DESTDIR)$(HOME)/.android/

.PHONY: clean
clean:
	rm -f 51-android.rules adb_usb.ini

