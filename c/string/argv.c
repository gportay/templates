/*
 *  Copyright (C) 2017-2018 Savoir-Faire Linux Inc.
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

#define _GNU_SOURCE

#ifdef HAVE_CONFIG_H
# include "config.h"
#else
const char VERSION[] = __DATE__ " " __TIME__;
#endif /* HAVE_CONFIG_H */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <getopt.h>

#include "argv.h"

struct options_t {
	size_t strsize;
	size_t nargv;
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
	fprintf(f, "Usage: %s [OPTIONS] ARG\n\n"
		   "Options:\n"
		   " -s or --strsize SIZE  Set string size.\n"
		   " -n or --nargv NUMBER  Set n argv.\n"
		   " -h or --help          Display this message.\n"
		   " -V or --version       Display the version.\n"
		   "", applet(arg0));
}

int parse_arguments(struct options_t *opts, int argc, char * const argv[])
{
	static const struct option long_options[] = {
		{ "strsize", required_argument, NULL, 's' },
		{ "nargv",   required_argument, NULL, 'n' },
		{ "version", no_argument,       NULL, 'V' },
		{ "help",    no_argument,       NULL, 'h' },
		{ NULL,      no_argument,       NULL, 0   }
	};

	opts->strsize = 32;
	opts->nargv = 4;
	for (;;) {
		int index;
		int c = getopt_long(argc, argv, "s:n:Vh", long_options, &index);
		if (c == -1) {
			break;
		}

		switch (c) {
		case 's': {
			char *e;
			long l = strtol(optarg, &e, 0);
			if (*e) {
				fprintf(stderr, "Error: Invalid long argument \"%s\"!\n", optarg);
				exit(EXIT_FAILURE);
			}
			opts->strsize = l;
			break;
		}

		case 'n': {
			char *e;
			long l = strtol(optarg, &e, 0);
			if (*e) {
				fprintf(stderr, "Error: Invalid long argument \"%s\"!\n", optarg);
				exit(EXIT_FAILURE);
			}
			opts->nargv = l;
			break;
		}

		case 'V':
			printf("%s\n", VERSION);
			exit(EXIT_SUCCESS);
			break;

		case 'h':
			usage(stdout, argv[0]);
			exit(EXIT_SUCCESS);
			break;

		case '?':
			exit(EXIT_FAILURE);
			break;

		default:
			fprintf(stderr, "Error: Illegal option code 0x%x!\n", c);
			exit(EXIT_FAILURE);
		}
	}

	return optind;
}

int main(int argc, char * const argv[])
{
	static struct options_t options;
	int argi, ret = EXIT_FAILURE;
	void *ptr = NULL;

	argi = parse_arguments(&options, argc, argv);
	if (argi < 0) {
		fprintf(stderr, "Error: Invalid argument!\n");
		exit(EXIT_FAILURE);
	}

	if (!strcmp(argv[0], "strtonargv")) {
		char buf[BUFSIZ], **arg, **p;
		ssize_t s, l = 0;

		arg = malloc(options.nargv * sizeof(char *));
		if (!arg) {
			perror("malloc");
			goto exit;
		}
		ptr = arg;

		l = 0;
		for (;;) {
			s = read(STDIN_FILENO, &buf[l], sizeof(BUFSIZ));
			if (s == -1) {
				perror("read");
				goto exit;
			} else if (!s) {
				break;
			}
			l += s;
		}
		buf[l] = '\0';

#if 0
s = write(STDERR_FILENO, buf, l);
if (s == -1) {
	perror("write");
	goto exit;
}
fprintf(stderr, "\n");
fprintf(stderr, "s: %li bytes\n", s);
#endif

		p = strtonargv(arg, buf, options.nargv);
		if (!p) {
			perror("strtonargv");
			goto exit;
		}

		printf("%s", *p++);
		while (*p)
			printf(" %s", *p++);
		printf("\n");

		ret = EXIT_SUCCESS;
	} else if (!strcmp(argv[0], "argvtonstr")) {
		char *str, *p;
		ssize_t s;

		str = malloc(options.strsize * sizeof(char));
		if (!str) {
			perror("malloc");
			goto exit;
		}
		ptr = str;

#if 0
		p = argvtonstr(str, &argv[argi], options.strsize);
		if (!p) {
			perror("argvtonstr");
			goto exit;
		}
#else
		p = str;
		(void)p;
		s = aswrite(str, &argv[argi], options.strsize);
		if (s == -1) {
			perror("write_argv");
			goto exit;
		}
#endif

#if 0
		s = write(STDOUT_FILENO, p, options.strsize * sizeof(char));
#else
		s = write(STDOUT_FILENO, str, s);
#endif
		if (s == -1) {
			perror("write");
			goto exit;
		}

		ret = EXIT_SUCCESS;
	}


exit:
	if (ptr) {
		free(ptr);
		ptr = NULL;
	}

	return ret;
}

/*
 * ex: filetype=c
 */
