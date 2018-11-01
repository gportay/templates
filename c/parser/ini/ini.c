/*
 * Copyright (C) 2017  Vivien Didelot
 *               2017-2018  Gaël Portay
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

#include "ini.h"

#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

static int ini_parse_line(char *line,
			  ini_section_cb_t *sec_cb,
			  ini_property_cb_t *prop_cb,
			  void *data)
{
	while (isblank(*line))
		line++;

	/* comment or empty line? */
	if (*line == '\0' || *line == ';' || *line == '#')
		return 0;

	/* section? */
	if (*line == '[') {
		char *end, *section;

		end = strchr(line, ']');
		if (!end) {
			fprintf(stderr, "malformated section: \"%s\".\n", line);
			return 1;
		}

		if (*(end + 1) != '\0') {
			fprintf(stderr, "trailing characters: \"%s\".\n", end);
			return 1;
		}

		section = line + 1;
		*end = '\0';

		if (!sec_cb)
			return 0;

		return sec_cb(section, data);
	}

	/* property? */
	if (isalnum(*line)) {
		char *equal, *key, *value;

		equal = strchr(line, '=');
		if (!equal) {
			fprintf(stderr, "malformated property: \"%s\"."
					" Must be a key=value pair!\n", line);
			return 1;
		}

		key = line;
		value = equal + 1;
		*equal = '\0';

		if (!prop_cb)
			return 0;

		return prop_cb(key, value, data);
	}

	fprintf(stderr, "line ignored: \"%s\".", line);
	return 1;
}

int ini_parse(int fd,
	      ini_section_cb_t *sec_cb,
	      ini_property_cb_t *prop_cb,
	      void *data)
{
	int skip;
	char buf[BUFSIZ+1];
	char *p, *nl;
	ssize_t len, nr;

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
				if (ini_parse_line(p, sec_cb, prop_cb, data))
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
