#
# The MIT License (MIT)
#
# Copyright (c) 2015-2017 Gaël PORTAY <gael.portay@gmail.com>
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#

# Enable ELF binary format support
LINUX_CONFIGS	+= CONFIG_BINFMT_ELF=y

.PHONY: all
all:

.PHONY: download
download: busybox_download

.PHONY: clean
clean: busybox_clean

.PHONY: mrproper
mrproper: busybox_mrproper

.PHONY: busybox
busybox: busybox/busybox

.SILENT: busybox/busybox
busybox/busybox: busybox/.config
	echo "Compiling busybox..."
	$(MAKE) -C busybox CONFIG_STATIC=y

.SILENT: busybox/.config
busybox/.config: busybox/Makefile
	echo "Configuring busybox..."
	yes "" | $(MAKE) -C busybox oldconfig

.SILENT: busybox/Makefile
busybox/Makefile:
	echo "You need to provide your own busybox sources into the $(CURDIR)/$(@D) directory!" >&2
	echo "Have a look at https://busybox.net! or run one of the commands below:" >&2
	echo "$$ git clone git://git.busybox.net/busybox.git $(CURDIR)/$(@D)" >&2
	echo "or" >&2
	echo "$$ $(MAKE) $(@D)_download" >&2
	exit 1

.SILENT: busybox_download
busybox_download:
	wget -qO- https://www.busybox.net | \
	sed -n '/<li><b>.* -- BusyBox .* (stable)<\/b>/,/<\/li>/{/<p>/s,.*<a.*href="\(.*\)">BusyBox \(.*\)</a>.*,wget -qO- \1 | tar xvj \&\& ln -sf busybox-\2 busybox,p}' | \
	head -n 1 | \
	$(SHELL)

.PHONY: rootfs/bin/busybox
ramfs/bin/busybox: busybox/busybox
	echo "Installing busybox..."
	$(MAKE) -C busybox install CONFIG_STATIC=y CONFIG_PREFIX=$(CURDIR)/ramfs/

busybox_menuconfig:
busybox_%:
	$(MAKE) -C busybox $*

.PHONY: busybox_clean
busybox_clean:

.PHONY: busybox_mrproper
busybox_mrproper:
	-$(MAKE) -C busybox distclean

