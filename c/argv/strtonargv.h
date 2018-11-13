/*
 *  Copyright (C) 2018 Gaël PORTAY
 *                2018 Savoir-Faire Linux Inc.
 *
 *  Authors:
 *       Gaël PORTAY <gael.portay@savoirfairelinux.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __STRTONARGV_H__
#define __STRTONARGV_H__

#include <stddef.h>
#include <errno.h>

static char *CFS = " \t\n"; /* Command-line Field Separator */
char *strchr(const char *s, int c);

static inline
char **strtonargv(char *dest[], char *src, int *n)
{
	char **arg = dest;
	char *str = src;
	char *s = NULL;
	int i = 0;

	if (!n || *n < 0) {
		errno = EINVAL;
		return NULL;
	}

	for (;;) {
		if (!*n)
			break;

		s = strchr(str, CFS[0]);
		if (!s)
			break;

		if (arg)
			*s = '\0'; /* CFS[0] <- NUL */
		s++; /* s = next cstring */
		(*n)--;
		i++;
		if (arg)
			*arg++ = str;
		str = s;
	}

	if (*str) {
		i++;
		if (arg)
			*arg++ = str;
	}

	if (arg)
		*arg = NULL;

	*n = i;
	return dest;
}

#endif /* __STRTONARGV_H__ */
