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

/*
 * cc -pie -fPIC -Wl,-E library.c -shared -pie -o library.so
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#else
static const char VERSION[] = __DATE__ " " __TIME__;
#endif /* HAVE_CONFIG_H */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int entrypoint(int argc, char * const argv[])
{
	int i;

	printf("argc: %i\n", argc);
	for (i = 0; i < argc; i++)
		printf("argv[%i]: %s\n", i, argv[i]);

	return 0;
}

int main(int argc, char * const argv[])
{
	printf("%s\n", VERSION);
	return entrypoint(argc, argv);
}
