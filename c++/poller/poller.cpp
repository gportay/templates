/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 Gaël PORTAY <gael.portay@gmail.com>
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
#include <errno.h>
#include <getopt.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <exception>

#include "errno_error.h"
#include "Watcher.h"
#include "Poller.h"

static int DEBUG = 0;
static int VERBOSE = 0;
#define verbose(fmt, ...) if (VERBOSE) fprintf(stdout, fmt, ##__VA_ARGS__)
#define debug(fmt, ...) if (DEBUG) fprintf(stderr, fmt, ##__VA_ARGS__)

struct options_t {
	;
};

static inline const char *name(const char *arg0)
{
	const char *s = strrchr(arg0, '/');

	if (!s)
		return arg0;

	return ++s;
}

void usage(FILE * f, char * const arg0)
{
	fprintf(f, "Usage: %s [OPTIONS]\n"
		   "Options:\n"
		   " -v or --verbose    Turn on verbose messages.\n"
		   " -D or --debug      Turn on debug messages.\n"
		   " -V or --version    Display the version.\n"
		   " -h or --help       Display this message.\n"
		   "", name(arg0));
}

int parse_arguments(struct options_t *opts, int argc, char * const argv[])
{
	static const struct option long_options[] = {
		{ "verbose", no_argument,       NULL, 'v' },
		{ "debug",   no_argument,       NULL, 'D' },
		{ "version", no_argument,       NULL, 'V' },
		{ "help",    no_argument,       NULL, 'h' },
		{ NULL,      no_argument,       NULL, 0   }
	};

	for (;;) {
		int option_index;
		int c = getopt_long(argc, argv, "vDVh", long_options, &option_index);
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

class Input : public Watcher
{
public:
	Input() : Watcher(STDIN_FILENO)
	{
		int flags = fcntl(_fd, F_GETFL);
		if (flags < 0)
			THROW_ERRNO_ERROR("fcntl");

		fcntl(_fd, F_SETFL, flags | O_NONBLOCK);
		if (flags < 0)
			THROW_ERRNO_ERROR("fcntl");
	}

	virtual ~Input()
	{
	}

	void onEvent(uint32_t events)
	{
		debug("%s(events: %u)\n", __func__, events);
		if (events | Poller::Event::INPUT) {
			for (;;) {
				char buf[16];
				ssize_t s = read(_fd, buf, sizeof(buf)-1);
				if (s < 0) {
					if (errno == EAGAIN) break;
					THROW_ERRNO_ERROR("read");
				}
				else if (!s) {
					break;
				}

				buf[s] = 0;
				printf("%s", buf);
			}
		}
	}
};

int main(int argc, char * const argv[])
{
	int ret = EXIT_FAILURE;
	static struct options_t options;

	int argi = parse_arguments(&options, argc, argv);
	if (argi < 0) {
		fprintf(stderr, "Error: Too few arguments!\n");
		exit(EXIT_FAILURE);
	} else if ((argc - argi) > 0) {
		usage(stderr, argv[0]);
		fprintf(stderr, "Error: Too many arguments!\n");
		exit(EXIT_FAILURE);
	}

	try {
		Input input;
		input.waitFor(Poller::Event::INPUT);
		Poller::get().loop();
	}
	catch (std::exception &e) {
		fprintf(stderr, "Error: %s!\n", e.what());
	}

exit:
	return ret;
}

