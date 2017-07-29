/*
 * Copyright (C) 2017 Gaël PORTAY <gael.portay@savoirfairelinux.com>
 *
 * This file is part of barebox.
 * See file CREDITS for list of people who contributed to this project.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

/** @file
 *  This File contains declaration for early output support
 */
#ifndef __MT7628_DEBUG_LL_H__
#define __MT7628_DEBUG_LL_H__

/*
 * 1000.0C00 - 1000.0CFF 256 Bytes UARTLITE
 * 1000.0D00 - 1000.0DFF 256 Bytes UART1
 * 1000.0E00 - 1000.0EFF 256 Bytes UART2
 */
#define MT7628_UARTLITE_BASE	0x10000c00
#define MT7628_UART1_BASE	0x10000d00
#define MT7628_UART2_BASE	0x10000e00

#define DEBUG_LL_UART_ADDR	MT7628_UART2_BASE
#define DEBUG_LL_UART_SHIFT	2

#define DEBUG_LL_UART_CLK       40000000
#define DEBUG_LL_UART_BPS       CONFIG_BAUDRATE
#define DEBUG_LL_UART_DIVISOR   (DEBUG_LL_UART_CLK / DEBUG_LL_UART_BPS)

#endif /* __MT7628_DEBUG_LL_H__ */
