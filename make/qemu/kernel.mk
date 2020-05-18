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

CONFIG_INITRAMFS_SOURCE ?=

# Enable 64-bits support for x86 target
ifeq (x86_64,$(shell uname -m))
LINUX_CONFIGS	+= CONFIG_64BIT=y
endif

# Enable printk support for traces
LINUX_CONFIGS	+= CONFIG_PRINTK=y
LINUX_CONFIGS	+= CONFIG_EARLY_PRINTK=y

.PHONY: all
all:

.PHONY: download
download: linux_download

.PHONY: clean
clean: kernel_clean

.PHONY: mrproper
mrproper: kernel_mrproper

bzImage: linux/arch/x86/boot/bzImage
	cp $< $@

.SILENT: linux/arch/x86/boot/bzImage
linux/arch/x86/boot/bzImage:

ifneq (,$(CONFIG_INITRAMFS_SOURCE))
linux/arch/x86/boot/bzImage: initramfs.cpio
endif

linux/arch/x86/boot/bzImage: linux/.config
	echo "Compiling linux ($(@F))..."
	$(MAKE) -C linux $(@F)

.SILENT: linux/.config
linux/.config: linux/Makefile kernel.cfg
	echo "Configuring linux using tinyconfig..."
	yes | $(MAKE) -C linux tinyconfig
	cd linux && scripts/kconfig/merge_config.sh $(@F) $(CURDIR)/kernel.cfg

.SILENT: linux/Makefile
linux/Makefile:
	echo "You need to provide your own kernel sources into the $(CURDIR)/$(@D) directory!" >&2
	echo "Have a look at https://www.kernel.org! or run one of the commands below:" >&2
	echo "$$ git clone git@github.com:torvalds/linux.git $(CURDIR)/$(@D)" >&2
	echo "or" >&2
	echo "$$ $(MAKE) $(@D)_download" >&2
	exit 1

.SILENT: linux_download
linux_download:
	wget -qO- https://www.kernel.org/index.html | \
	sed -n '/<td id="latest_link"/,/<\/td>/s,.*<a.*href="\(.*\)">\(.*\)</a>.*,wget -qO- \1 | tar xvJ \&\& ln -sf linux-\2 linux,p' | \
	$(SHELL)

kernel.cfg:
	for cfg in $(LINUX_CONFIGS); do \
		echo $$cfg; \
	done >$@

.PHONY: kernel_clean
kernel_clean:
	rm -f kernel.cfg bzImage

.PHONY: kernel_mrproper
kernel_mrproper: kernel_clean
	-$(MAKE) -C linux distclean

.PHONY:
kernel: bzImage

linux_menuconfig:
linux_%:
	$(MAKE) -C linux $*

