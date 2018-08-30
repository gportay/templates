/*
 * Copyright 2016-2018 Gaël PORTAY <gael.portay@gmail.com>
 *
 * Licensed under the MIT license.
 *
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#else
const char VERSION[] = __DATE__ " " __TIME__;
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <getopt.h>

#include "verrevcmp.h"

static int DEBUG = 0;
#define debug(fmt, ...)  if (DEBUG)      printf(fmt, ##__VA_ARGS__)
#define debug2(fmt, ...) if (DEBUG >= 2) printf(fmt, ##__VA_ARGS__)

struct options {
	int equal;
	int quiet;
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
	fprintf(f, "Usage: %s [OPTIONS] VERS VERS_REF\n\n"
		   "Prints the Lower version of VERS and VERS_REF.\n\n"
		   "Exits success if VERS is Lower Than (lt) VERS_REF.\n\n"
		   "Options:\n"
		   " -e or --equal         Lower or Equal.\n"
		   " -q or --quiet         Quiet.\n"
		   " -D or --debug         Turn on debug traces.\n"
		   " -h or --help          Display this message.\n"
		   " -V or --version       Display the version.\n"
		   "", applet(arg0));
}

int parse_arguments(struct options *opts, int argc, char * const argv[])
{
	static const struct option long_options[] = {
		{ "equal",   no_argument,       NULL, 'e' },
		{ "quiet",   no_argument,       NULL, 'q' },
		{ "debug",   no_argument,       NULL, 'D' },
		{ "help",    no_argument,       NULL, 'h' },
		{ "version", no_argument,       NULL, 'V' },
		{  NULL,     no_argument,       NULL,  0  }
	};

	for (;;) {
		int option_index;
		int c = getopt_long(argc, argv, "eqDhV", long_options, &option_index);
		if (c == -1) {
			break;
		}

		switch (c) {
		case 'e':
			opts->equal = 1;
			break;

		case 'q':
			opts->quiet = 1;
			break;

		case 'D':
			DEBUG++;
			break;

		case 'h':
			usage(stdout, argv[0]);
			exit(EXIT_SUCCESS);
			break;

		case 'V':
			printf("%s\n", VERSION);
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
	int cmp, ret = EXIT_FAILURE;
	static struct options options;

	int argi = parse_arguments(&options, argc, argv);
	if (argi < 0) {
		exit(EXIT_FAILURE);
	}
	else if (argc - argi < 1) {
		usage(stdout, argv[0]);
		fprintf(stderr, "Error: Too few arguments!\n");
		exit(EXIT_FAILURE);
	}
	else if (argc - argi > 2) {
		usage(stdout, argv[0]);
		fprintf(stderr, "Error: Too many arguments!\n");
		exit(EXIT_FAILURE);
	}

	cmp = verrevcmp(argv[argi], argv[argi+1]);
	debug2("verrevcmp(VERS: %s, VERS_REF: %s): %i\n", argv[argi], argv[argi+1], cmp);
	if (options.equal) {
		debug2("Lower or Equal\n");
		ret = (cmp <= 0) ? EXIT_SUCCESS : EXIT_FAILURE;
	}
	else {
		ret = (cmp <  0) ? EXIT_SUCCESS : EXIT_FAILURE;
	}

	debug("%s is%s lower%s than %s!\n",
              argv[argi],
	      (ret == EXIT_SUCCESS) ? "" : " not",
	      (options.equal == 0)  ? "" : " or equal",
	      argv[argi+1]);
	if (ret == EXIT_SUCCESS && !options.quiet)
		printf("%s\n", (cmp < 0) ? argv[argi] : argv[argi+1]);

	return ret;
}
