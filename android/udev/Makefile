#
# Copyright 2016 Gaël PORTAY <gael.portay@gmail.com>
#
# Licensed under the MIT license.
#

USER	?= $(shell whoami)
GROUP	?= android
RULES	:= $(wildcard *.rules)
RULES	:= $(filter-out 51-android.rules,$(RULES))

all: 51-android.rules

51-android.rules: $(RULES)
	wget -S -O - http://source.android.com/source/$@ | \
	sed -e "s/MODE=\"0600\"/MODE=\"0644\"/" \
	    -e "s/OWNER/GROUP/" \
	    -e "s/<username>/$(GROUP)/" | cat - $? | \
	tee $@

setup:
	groupadd --force --system $(GROUP)
	usermod --append --groups $(GROUP) $(USER)

install: 51-android.rules
	install -m 0755 -d $(DESTDIR)/etc/udev/rules.d/
	install -m 0644 $< $(DESTDIR)/etc/udev/rules.d/

reload:
	udevadm control --reload-rules
clean:
	rm -f 51-android.rules

