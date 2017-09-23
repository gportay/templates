/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015-2017 Gaël PORTAY <gael.portay@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
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

struct options_t {
	const char *string;
	long number;
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
		   " -s or --string STRING Set string.\n"
		   " -n or --number NUMBER Set number.\n"
		   " -h or --help          Display this message.\n"
		   " -V or --version       Display the version.\n"
		   "", applet(arg0));
}

int parse_arguments(struct options_t *opts, int argc, char * const argv[])
{
	static const struct option long_options[] = {
		{ "string",  required_argument, NULL, 's' },
		{ "number",  required_argument, NULL, 'n' },
		{ "version", no_argument,       NULL, 'V' },
		{ "help",    no_argument,       NULL, 'h' },
		{ NULL,      no_argument,       NULL, 0   }
	};

	for (;;) {
		int index;
		int c = getopt_long(argc, argv, "s:n:Vh", long_options, &index);
		if (c == -1) {
			break;
		}

		switch (c) {
		case 's':
			opts->string = optarg;
			break;

		case 'n': {
			char *e;
			long l = strtol(optarg, &e, 0);
			if (*e) {
				fprintf(stderr, "Error: Invalid long argument \"%s\"!\n", optarg);
				exit(EXIT_FAILURE);
			}
			opts->number = l;
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
	int ret = EXIT_FAILURE;
	static struct options_t options;

	int argi = parse_arguments(&options, argc, argv);
	if (argi < 0) {
		fprintf(stderr, "Error: Invalid argument!\n");
		exit(EXIT_FAILURE);
	} else if (argc - argi < 1) {
		usage(stdout, argv[0]);
		fprintf(stderr, "Error: Too few arguments!\n");
		exit(EXIT_FAILURE);
	} else if (argc - argi > 1) {
		usage(stdout, argv[0]);
		fprintf(stderr, "Error: Too many arguments!\n");
		exit(EXIT_FAILURE);
	}

	printf("arg: %s, string: \"%s\", number: %li\n", argv[1], options.string, options.number);

exit:
	return ret;
}
