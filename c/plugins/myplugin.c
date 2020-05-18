/*
 *  Copyright (C) 2018 Gaël PORTAY
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

#include <unistd.h>
#include <stdio.h>

#include <plugin.h>

int myentrypoint(int argc, char * const argv[])
{
	int i;

	printf("%s { argc: %i, argv[]: { %s", __func__, argc, *argv);
	for (i = 1; i < argc; i++) {
		printf(", %s", argv[i]);
	}
	printf(" } }\n");
	
	return sleep(1);
}

DECLARE_PLUGIN(myentrypoint);
