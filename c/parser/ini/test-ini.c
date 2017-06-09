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

#include "ini.h"

#include <unistd.h>
#include <stdio.h>

static int section(char *section, void *data)
{
	fprintf((FILE*)data, "[%s]\n", section);
	return 0;
}

static int property(char *key, char *value, void *data)
{
	fprintf((FILE*)data, "%s=%s\n", key, value);
	return 0;
}

int main(void)
{
	return ini_parse(STDIN_FILENO, section, property, stdout);
}
