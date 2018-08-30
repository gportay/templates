/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016-2018 Gaël PORTAY <gael.portay@gmail.com>
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
#include "config.h"
#else
const char VERSION[] = __DATE__ " " __TIME__;
#endif /* HAVE_CONFIG_H */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <regex.h>
#include <getopt.h>

#include "iana-mimes.h"

static int DEBUG = 0;
static int VERBOSE = 0;
#define verbose(fmt, ...) if (VERBOSE) fprintf(stdout, fmt, ##__VA_ARGS__)
#define debug(fmt, ...) if (DEBUG) fprintf(stderr, fmt, ##__VA_ARGS__)

struct options {
	;
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
	fprintf(f, "Usage: %s [OPTIONS] REGEX\n"
		   "Options:\n"
		   " -v or --verbose    Turn on verbose messages.\n"
		   " -D or --debug      Turn on debug messages.\n"
		   " -V or --version    Display the version.\n"
		   " -h or --help       Display this message.\n"
		   "", applet(arg0));
}

int parse_arguments(struct options *opts, int argc, char * const argv[])
{
	static const struct option long_options[] = {
		{ "verbose", no_argument,       NULL, 'v' },
		{ "debug",   no_argument,       NULL, 'D' },
		{ "version", no_argument,       NULL, 'V' },
		{ "help",    no_argument,       NULL, 'h' },
		{ NULL,      no_argument,       NULL, 0   }
	};

	for (;;) {
		int index;
		int c = getopt_long(argc, argv, "vDVh", long_options, &index);
		if (c == -1) {
			break;
		}

		switch (c) {
		case 'v':
			VERBOSE++;
			break;

		case 'D':
			DEBUG++;
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
	static struct options options;
	const char **mime;
	char error[128];
	regex_t regex;

	int argi = parse_arguments(&options, argc, argv);
	if (argi < 1) {
		fprintf(stderr, "Error: Invalid argument!\n");
		exit(EXIT_FAILURE);
	} else if ((argc - argi) < 1) {
		usage(stderr, argv[0]);
		fprintf(stderr, "Error: Too few arguments!\n");
		exit(EXIT_FAILURE);
	} else if ((argc - argi) > 1) {
		usage(stderr, argv[0]);
		fprintf(stderr, "Error: Too many arguments!\n");
		exit(EXIT_FAILURE);
	}

	ret = regcomp(&regex, argv[argi], 0);
	if (ret) {
		regerror(ret, &regex, error, sizeof(error));
		fprintf(stderr, "regcomp: %s\n", error);
		exit(EXIT_FAILURE);
	}

	mime = IANA_MIMES;
	while (*mime) {
		ret = regexec(&regex, *mime, 0, NULL, 0);
		if ((ret != REG_NOERROR) && (ret != REG_NOMATCH)) {
			regerror(ret, &regex, error, sizeof(error));
			fprintf(stderr, "regexec: %s\n", error);
			ret = EXIT_FAILURE;
			goto exit;
		} else if (ret == REG_NOERROR) {
			printf("%s\n", *mime);
		}
		mime++;
	}
	ret = EXIT_SUCCESS;

exit:
	regfree(&regex);
	return ret;
}
