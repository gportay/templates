/*
 * Copyright (C) 2017  Vivien Didelot
 *                     Gaël Portay
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

#include "line.h"

#include <unistd.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int line_parse(int fd, line_cb_t *line_cb, void *data)
{
	int skip;
	char buf[BUFSIZ+1];
	char *p, *nl;
	ssize_t len, nr;

	if (!line_cb)
		return 1;

	skip = 0;
	len = 0;
	for (;;) {
		/* Read chunk */
		nr = read(fd, buf + len, sizeof(buf) - len - 1);
		if (nr == -1) {
			perror("read");
			return 1;
		}

		if (nr == 0)
			break;

		/* Process lines */
		buf[len+nr] = '\0';
		p = buf;
		for (;;) {
			nl = strchr(p, '\n');
			if (!nl)
				break;

			*nl = '\0';
			if (!skip) {
				if (line_cb(p, nl - p, data))
					return 1;
			} else {
				skip = 0;
			}

			p = nl + 1;
		}

		/* Move remaining data to the begining of the buffer */
		len = strlen(p);
		if (len == sizeof(buf) - 1) {
			if (!skip)
				fprintf(stderr, "line ignored: \"%s\"."
						" Must be shorter than %ld!\n",
						p, sizeof(buf) - 1);
			len = 0;
			skip = 1;
		}
		strcpy(buf, p);
	}

	return 0;
}
