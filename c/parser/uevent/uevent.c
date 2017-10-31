/*
 * Copyright (C) 2017  Gaël Portay
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "uevent.h"

#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

#define __memcmp(s1, s2) memcmp(s1, s2, sizeof(s2) - 1)

#ifndef UEVENT_BUFFER_SIZE
#define UEVENT_BUFFER_SIZE 2048
#endif

static int uevent_parse_line(char *line,
			     uevent_event_cb_t *evt_cb,
			     uevent_variable_cb_t *var_cb,
			     void *data)
{
	char *at, *equal;

	/* empty line? */
	if (*line == '\0')
		return 0;

	/* event? */
	at = strchr(line, '@');
	if (at) {
		char *action, *devpath;

		action = line;
		devpath = at + 1;
		*at = '\0';

		if (!evt_cb)
			return 0;

		return evt_cb(action, devpath, data);
	}

	/* variable? */
	equal = strchr(line, '=');
	if (equal) {
		char *variable, *value;

		variable = line;
		value = equal + 1;
		*equal = '\0';

		if (!var_cb)
			return 0;

		return var_cb(variable, value, data);
	}

	fprintf(stderr, "malformated event or variable: \"%s\"."
			" Must be either action@devpath,\n"
			"             or variable=value!\n", line);
	return 1;
}

int uevent_parse(int fd,
		 uevent_event_cb_t *evt_cb,
	         uevent_variable_cb_t *var_cb,
	         void *data)
{
	char buf[UEVENT_BUFFER_SIZE+1];
	char *p, *nl;
	ssize_t nr;

	for (;;) {
		/* Read uevent */
		nr = read(fd, buf, sizeof(buf) - 1);
		if (nr == -1) {
			perror("read");
			return 1;
		}

		if (nr == 0)
			break;

		/* Skip libudev */
		if (!__memcmp(buf, "libudev"))
			continue;

		/* Process lines */
		buf[nr] = '\0';
		p = buf;
		for (;;) {
			nl = strchr(p, '\0');
			if (!nl || nl == p)
				break;

			if (uevent_parse_line(p, evt_cb, var_cb, data))
				return 1;

			p = nl + 1;
		}
	}

	return 0;
}
