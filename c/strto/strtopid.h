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

#ifndef __STRTOPID_H__
#define __STRTOPID_H__

#include <stddef.h>
#include <stdlib.h>
#include <errno.h>

static inline pid_t strtopid(const char *nptr)
{
	pid_t pid = -1;
	int olderrno;
	char *endptr;

	olderrno = errno;
	errno = 0;
	pid = strtol(nptr, &endptr, 0);
	if (pid <= 0 || errno || *endptr) {
		errno = EINVAL;
		pid = -1;
	} else {
		errno = olderrno;
	}

	return pid;
}

#endif /* __STRTOPID_H__ */
