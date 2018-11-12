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

#ifdef HAVE_CONFIG_H
# include "config.h"
#else
const char VERSION[] = __DATE__ " " __TIME__;
#endif /* HAVE_CONFIG_H */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <getopt.h>

#include "strtonargv.h"

struct options {
	int argc;
	char * const *argv;
	int count;
};

static inline const char *applet(const char *arg0)
{
	char *s = strrchr(arg0, '/');
	if (!s)
		return arg0;

	return s+1;
}

void usage(FILE * f, char * const arg0)
{
	fprintf(f, "Usage: %s [OPTIONS] STRINGS...\n"
		   "\n"
		   "strtonargv(1) splits strings to a list of words.\n"
		   "\n"
		   "The separators are defined by environment variables. CFS "
		   "splits strings into\nwords. Words are expanded using the "
		   "first character of IFS.\n"
		   "\n"
		   "Options:\n"
		   " --count                Count only.\n"
		   " -h or --help           Display this message.\n"
		   " -V or --version        Display the version.\n"
		   "\n"
		   "Environment Variables:\n"
		   " CFS                    Common Field Separator to split\n"
		   "                        strings into words [default=' ']\n"
		   " IFS                    Internal Field Separator to.\n"
		   "                        expands words [default=' \\t\\n']\n"
		   "", applet(arg0));
}

int parse_arguments(struct options *opts, int argc, char * const argv[])
{
	static const struct option long_options[] = {
		{ "count",   no_argument,       NULL, 1   },
		{ "version", no_argument,       NULL, 'V' },
		{ "help",    no_argument,       NULL, 'h' },
		{ NULL,      no_argument,       NULL, 0   }
	};

	opterr = 0;
	opts->argc = 0;
	opts->argv = NULL;
	opts->count = 0;
	for (;;) {
		int index;
		int c = getopt_long(argc, argv, "n:Vh", long_options, &index);
		if (c == -1) {
			break;
		}

		switch (c) {
		case 1:
			opts->count = 1;
			break;

		case 'V':
			printf("%s\n", VERSION);
			exit(EXIT_SUCCESS);
			break;

		case 'h':
			usage(stdout, argv[0]);
			exit(EXIT_SUCCESS);
			break;

		case '?':
			return -1;
		}
	}

	opts->argc = argc;
	opts->argv = argv;
	return optind;
}

int main(int argc, char * const argv[])
{
	static struct options options;
	int i, argi, ret = EXIT_FAILURE;
	char *cfs, *ifs, *IFS = " \t\n";

	argi = parse_arguments(&options, argc, argv);
	if (argi < 0) {
		fprintf(stderr, "Error: %s: Invalid option!\n", argv[optind-1]);
		exit(EXIT_FAILURE);
	} else if (argc - argi < 1) {
		usage(stdout, argv[0]);
		fprintf(stderr, "Error: Too few arguments!\n");
		exit(EXIT_FAILURE);
	}

	ifs = getenv("IFS");
	if (ifs)
		IFS = ifs;

	cfs = getenv("CFS");
	if (cfs)
		CFS = cfs;

	for (i = argi; i < argc; i++) {
		int nargs = 127;
		strtonargv(NULL, argv[i], &nargs);

		if (options.count) {
			printf("%i\n", nargs);
			continue;
		}

		if (nargs >= 0) {
			int j;
			char *args[nargs];

			if (!strtonargv(args, argv[i], &nargs)) {
				perror("strtonargv");
				goto exit;
			}

			printf("%s", args[0]);
			for (j = 1; j < nargs; j++)
				printf("%c%s", IFS[0], args[j]);
			printf("\n");
		}
	}

	ret = EXIT_SUCCESS;

exit:
	return ret;
}
