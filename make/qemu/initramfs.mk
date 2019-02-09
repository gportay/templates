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

# Enable initramfs/initrd support
LINUX_CONFIGS	+= CONFIG_BLK_DEV_INITRD=y
LINUX_CONFIGS	+= CONFIG_BLK_DEV_RAM=y

# /proc file system support
LINUX_CONFIGS	+= CONFIG_PROC_FS=y

# Posix Clocks & timers
LINUX_CONFIGS	+= CONFIG_POSIX_TIMERS=y

.PHONY: all
all:

.PHONY: clean
clean: initramfs_clean

.PHONY: mrproper
mrproper: initramfs_mrproper

include busybox.mk

initramfs.cpio: ramfs

ramfs ramfs/dev ramfs/proc ramfs/etc ramfs/root:
	mkdir -p $@

ramfs/init ramfs/linuxrc:
	ln -sf /bin/sh $@

ramfs/dev/initrd: | ramfs/dev
	fakeroot -i ramfs.env -s ramfs.env -- mknod -m 400 $@ b 1 250

ramfs/dev/console: | ramfs/dev
	fakeroot -i ramfs.env -s ramfs.env -- mknod -m 622 $@ c 5 1

ramfs/etc/passwd: | ramfs/etc
	echo "root::0:0:root:/root:/bin/sh" >$@

ramfs/etc/group: | ramfs/etc
	echo "root:x:0:root" >$@

initramfs.cpio.gz:

initramfs.cpio: | ramfs/proc
initramfs.cpio: ramfs/bin/busybox ramfs/dev/console

include init.mk

initramfs.cpio: ramfs/etc/passwd ramfs/etc/group | ramfs/root

%.cpio:
	cd $< && find . | \
	fakeroot -i $(CURDIR)/ramfs.env -s $(CURDIR)/ramfs.env -- \
	cpio -H newc -o -R root:root >$(CURDIR)/$@

%.gz: %
	gzip -9 $*

.PHONY: initramfs_clean
initramfs_clean:
	rm -Rf ramfs/ ramfs.env
	rm -f initramfs.cpio

.PHONY: initramfs_mrproper
initramfs_mrproper:

