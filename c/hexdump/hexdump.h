/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Gaël PORTAY <gael.portay@gmail.com>
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

#ifndef __HEXDUMP_H__
#define __HEXDUMP_H__

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdio.h>

static unsigned int ROWSIZE = 0x10;

static void hexdump_line(const unsigned char *buf, unsigned int size)
{
	unsigned int j;
	unsigned char b[ROWSIZE];
	memcpy(b, buf, size);
	memset(&b[size], 0x00, (ROWSIZE - size));

	for (j = 0; j < ROWSIZE; j++) {
		printf(" %02x", buf[j]);
	}

	printf("\t");

	for (j = 0; j < ROWSIZE; j++) {
		if ((buf[j] < 0x20) || (buf[j] >= 0x7F))
			printf(".");
		else
			printf("%c", (char) buf[j]);
	}

	printf("\n");
}

static void hexdump(unsigned int address, const unsigned char *buf, unsigned int size)
{
	unsigned int r, row;

	row = size / ROWSIZE;
	if (size % ROWSIZE)
		row++;

	printf("%s:", "@address");
	for (r = 0; r < ROWSIZE; r ++)
		printf(" %02x", r);
	printf("\n");

	for (r = 0; r < row; r++) {
		printf("%08x:", address + (r * ROWSIZE));
		hexdump_line(buf, ROWSIZE);
		buf += ROWSIZE;
	}
	printf("%08x\n", address + size);
}

#endif /* __HEXDUMP_H__ */
