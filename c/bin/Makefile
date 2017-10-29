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

TEXT_BASE = 0xbc000000
TEXT_BASE = 0x00000000
##  mipsel-unknown-linux-gnu-gcc -Wp,-MD,arch/mips/boot/.start.o.d  -nostdinc -isystem /home/gportay/x-tools/mipsel-unknown-linux-gnu/lib/gcc/mipsel-unknown-linux-gnu/6.1.0/include -D__KERNEL__ -D__BAREBOX__ -Iinclude -I/home/gportay/src/barebox-mips/dts/include  -I/home/gportay/src/barebox-mips/arch/mips/include -I/home/gportay/src/barebox-mips/arch/mips/include -include /home/gportay/src/barebox-mips/include/linux/kconfig.h -fno-builtin -ffreestanding -D__MIPS__ -fno-strict-aliasing -fno-merge-constants -DTEXT_BASE=0xbc000000 -fdata-sections -ffunction-sections -Iarch/mips/mach-mediatek/include -Iarch/mips/boards/vocore2//include -G 0 -mno-abicalls -fno-pic -pipe -Wall -Wmissing-prototypes -Wstrict-prototypes -Wno-uninitialized -Wno-format -Wno-main -mabi=32  -march=mips32r2 -Wa,-mips32r2 -Wa,--trap -D__ASSEMBLY__    -c -o arch/mips/boot/start.o arch/mips/boot/start.S
## -Wp,-MD,arch/mips/boot/.start.o.d
## -nostdinc -isystem /home/gportay/x-tools/mipsel-unknown-linux-gnu/lib/gcc/mipsel-unknown-linux-gnu/6.1.0/include
## -D__KERNEL__ -D__BAREBOX__
## -Iinclude -include /home/gportay/src/barebox-mips/include/linux/kconfig.h
## -fno-builtin -ffreestanding
## -D__MIPS__
## -fno-strict-aliasing -fno-merge-constants
## -DTEXT_BASE=0xbc000000
## -fdata-sections -ffunction-sections
## -G 0
## -mno-abicalls -fno-pic -pipe -Wall -Wmissing-prototypes -Wstrict-prototypes -Wno-uninitialized -Wno-format -Wno-main
## -mabi=32 -march=mips32r2 -Wa,-mips32r2 -Wa,--trap -D__ASSEMBLY__
ARCH := mips
CROSS_COMPILE := mipsel-unknown-linux-gnu-
CC := $(CROSS_COMPILE)$(CC)
OBJCOPY = $(CROSS_COMPILE)objcopy
MKIMAGE = mkimage
#CPPFLAGS += -D__ASSEMBLY__
CPPFLAGS += -D__KERNEL__ -DCONFIG_HAS_DEBUG_LL -DCONFIG_DEBUG_LL
CFLAGS += -std=c99 -Werror -Wextra -g
CFLAGS += -nostdinc
CFLAGS += -Iinclude
#CFLAGS += -isystem /home/gportay/x-tools/mipsel-unknown-linux-gnu/lib/gcc/mipsel-unknown-linux-gnu/6.1.0/include 
CFLAGS += -fno-builtin -ffreestanding
CLFAGS += -Wno-main
CLFAGS += -mno-abicalls -fno-pic
CLFAGS += -mabi=32 -march=mips32r2 -Wa,-mips32r2 -Wa,--trap
MKFLAGS += -A $(ARCH) -O u-boot -T standalone -C none -a $(TEXT_BASE) -e $(TEXT_BASE) -n "Standelone Image" 
#	   -A arch    -O os   -T type       -C comp -a addr         -e ep           -n name -d data_file[:data_file...] image

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
#	$(MKIMAGE) -A $(ARCH) -T standalone -C none \
#		   -a $(TEXT_BASE) -e $(TEXT_BASE) \
#		   -n "Hello World Image" \
#		   -r DDR2 -s 16 -t 64 -u 32 \
#		   -y 0xFF -z 0xFF -w 0xFF -d $< $@


# We don't want gcc reordering functions if possible.  This ensures that an
# application's entry point will be the first function in the application's
# source file.
#ccflags-y += $(call cc-option,-fno-toplevel-reorder)

#########################################################################

#quiet_cmd_link_lib = LD      $@
#      cmd_link_lib = $(LD) $(ld_flags) -r -o $@ $(filter $(LIBOBJS), $^)
#
#$(LIB):	$(LIBOBJS) FORCE
#	$(call if_changed,link_lib)

#quiet_cmd_link_elf = LD      $@
#      cmd_link_elf = $(LD) $(LDFLAGS) -g -Ttext $(CONFIG_STANDALONE_LOAD_ADDR) \
#		     -o $@ -e $(SYM_PREFIX)$(@F) $< $(LIB) $(PLATFORM_LIBGCC)
#
#$(ELF): $(obj)/%: $(obj)/%.o $(LIB) FORCE
#	$(call if_changed,link_elf)

#$(obj)/%.srec: OBJCOPYFLAGS := -O srec
#$(obj)/%.srec: $(obj)/% FORCE
#	$(call if_changed,objcopy)
#
#$(obj)/%.bin: OBJCOPYFLAGS := -O binary
#$(obj)/%.bin: $(obj)/% FORCE
#	$(call if_changed,objcopy)
#
