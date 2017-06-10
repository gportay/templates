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

typedef struct {
	int lines;
	int bytes;
} stats_t;

static int line(char *line, size_t size, void *data)
{
	stats_t *stats;

	stats = (stats_t *)data;
	stats->lines++;
	stats->bytes += size;

	fprintf(stdout, "%d: %s\n", stats->lines, line);

	return 0;
}

int main(void)
{
	stats_t stats;

	stats.lines = 0;
	stats.bytes = 0;
	line_parse(STDIN_FILENO, line, &stats);
	printf("%d lines.\n", stats.lines);
	printf("%d bytes.\n", stats.bytes);

	return 0;
}
