/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2017 Gaël PORTAY <gael.portay@savoirfairelinux.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <asm/io.h>

extern int puts(const char *s);

#define GPIO_BASE_ADDR	0x10000600
#define GPIO_DATA1	0x24

void main(void)
{
	/*
	 * root@OpenWrt:/# echo 1 >/sys/class/gpio/gpio44/value 
	 * root@OpenWrt:/# devmem 0x10000624 32
	 * 0x0000301F
	 * root@OpenWrt:/# echo 0 >/sys/class/gpio/gpio44/value 
	 * root@OpenWrt:/# devmem 0x10000624 32
	 * 0x0000201F
	 */
	u8 *reg = (u8 *)GPIO_BASE_ADDR + GPIO_DATA1;
	u32 val = __raw_readl(reg);
#if 0
	val &= ~0x1000;
	__raw_writeb(val, reg);
#else
	val |= 0x1000;
	__raw_writeb(val, reg);
#endif
	puts("Hello, World!\n");
}
