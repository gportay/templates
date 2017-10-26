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

CMDLINE	?=

# Enable console on 8250/16550 and compatible serial port
LINUX_CONFIGS	+= CONFIG_TTY=y
LINUX_CONFIGS	+= CONFIG_SERIAL_8250=y
LINUX_CONFIGS	+= CONFIG_SERIAL_8250_CONSOLE=y

.PHONY: all
all:

include initramfs.mk
include kernel.mk

.PHONY: runqemu
runqemu:

runqemu: KERNELFLAG=-kernel bzImage
runqemu: bzImage

runqemu: INITRDFLAG=-initrd initramfs.cpio
runqemu: initramfs.cpio

ifneq (,$(CMDLINE))
runqemu: APPENDFLAG=-append "$(CMDLINE)"
endif

runqemu:
	-ttysave="$$(stty -g)"; \
	qemu-system-$(shell uname -m) $(KERNELFLAG) $(INITRDFLAG) $(APPENDFLAG) -serial stdio; \
	stty $$ttysave

