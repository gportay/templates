#
# The MIT License (MIT)
#
# Copyright (c) 2017 Gaël PORTAY <gael.portay@savoirfairelinux.com>
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in thec Software without restriction, including without limitation the rights
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

TEXT_BASE = 0x00000000
CROSS_COMPILE := mipsel-unknown-linux-gnu-
CC := $(CROSS_COMPILE)$(CC)
OBJCOPY = $(CROSS_COMPILE)objcopy
MKIMAGE = mkimage
CPPFLAGS += -D__KERNEL__ -DCONFIG_HAS_DEBUG_LL -DCONFIG_DEBUG_LL
CFLAGS += -std=c99 -Werror -Wextra -g
CFLAGS += -nostdinc
CFLAGS += -Iinclude
CFLAGS += -fno-builtin -ffreestanding
CLFAGS += -Wno-main
CLFAGS += -mno-abicalls -fno-pic
CLFAGS += -mabi=32 -march=mips32r2 -Wa,-mips32r2 -Wa,--trap

.PHONY: all
all: helloworld.elf helloworld.srec helloworld.bin helloworld.img

helloworld: LDFLAGS += -Ttext $(TEXT_BASE)
helloworld: LDFLAGS += -nostdlib
helloworld: puts.o

.PHONY: help
help:
	@echo "U-Boot:"
	@echo "setenv bareboxcmd 'tftp \$${loadaddr} barebox.bin; go \$${loadaddr}'"
	@echo "setenv helloworldbincmd 'tftp \$${loadaddr} helloworld.bin; go \$${loadaddr}'"
	@echo "setenv helloworldimgcmd 'tftp \$${loadaddr} helloworld.img; go \$${loadaddr}'"
	@echo "saveenv"
	@echo "go \$${loadaddr}"
	@echo "bootm \$${loadaddr}"
	@echo ""
	@echo "Host:"
	@echo "ip addr add 192.168.1.3/24 dev enp0s25"
	@echo "picocom -b 115200 /dev/ttyACM0"

.PHONY: install
install:
	install -m 755 helloworld.elf helloworld.srec helloworld.bin helloworld.img /srv/tftp/

.PHONY: clean
clean:
	rm -f *.o *.elf *.srec *.bin *.img

.PHONY: debug
debug:
	$(MAKE) -spf /dev/null

%.i: %.c
	$(PREPROCESS.S) $< > $@

%.elf: %
	ln -sf $< $@

%.srec: %
	$(OBJCOPY) -O srec $< $@

%.bin: %
	$(OBJCOPY) -O binary $< $@

%.img: %.bin
	$(MKIMAGE) $(MKFLAGS) -d $< $@
