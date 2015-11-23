/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Gaël PORTAY <gael.portay@gmail.com>
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

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <getopt.h>
#include <sys/epoll.h>

#include "hexdump.h"

#define BUFSIZE 4096
#define __memset(ptr, c) memset(ptr, c, sizeof(ptr))

struct options_t {
	int timeout;
};

inline const char *applet(const char *arg0)
{
	char *s = strrchr(arg0, '/');

	if (!s)
		return arg0;

	return s+1;
}

void usage(FILE * f, char * const arg0)
{
	fprintf(f, "Usage: %s [OPTIONS]\n\n"
		   "Options:\n"
		   " -t or --timeout SECOND Set time-out in seconds [default=-1].\n"
		   " -s or --row-size SIZE  Set row size in bytes [default=16].\n"
		   " -h or --help           Display this message.\n"
		   " -v or --version        Display the version.\n"
		   "", applet(arg0));
}

int parse_arguments(struct options_t *opts, int argc, char * const argv[])
{
	static const struct option long_options[] = {
		{ "timeout",  required_argument, NULL, 't' },
		{ "row-size", required_argument, NULL, 's' },
		{ "version",  no_argument,       NULL, 'v' },
		{ "help",     no_argument,       NULL, 'h' },
		{ NULL,       no_argument,       NULL, 0   }
	};

	for (;;) {
		int option_index;
		int c = getopt_long(argc, argv, "t:s:vh", long_options, &option_index);
		if (c == -1) {
			break;
		}

		switch (c) {
		case 't': {
			char *e;
			long l = strtol(optarg, &e, 0);
			if (*e) {
				fprintf(stderr, "Error: Invalid time-out argument \"%s\"!\n", optarg);
				exit(EXIT_FAILURE);
			}
			if (l >= 0)
				opts->timeout = l * 1000;
			else
				opts->timeout = -1;
			break;
		}

		case 's': {
			char *e;
			unsigned long ul = strtoul(optarg, &e, 0);
			if (*e) {
				fprintf(stderr, "Error: Invalid size argument \"%s\"!\n", optarg);
				exit(EXIT_FAILURE);
			}
			ROWSIZE = ul;
			break;
		}

		case 'v':
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
	int pfd = -1;
	int fd = -1;
	int ret = EXIT_FAILURE;
	struct epoll_event epoll_event;
	static struct options_t options = {
		.timeout = -1,
	};
	size_t size = 0;

	int argi = parse_arguments(&options, argc, argv);
	if (argi < 0) {
		exit(EXIT_FAILURE);
	}
	else if (argc - argi >= 1) {
		usage(stdout, argv[0]);
		fprintf(stderr, "Error: Too many arguments!\n");
		exit(EXIT_FAILURE);
	}

	pfd = epoll_create(1);
	if (pfd < 0) {
		perror("epoll_create");
		exit(EXIT_FAILURE);
	}

	fd = STDIN_FILENO;
	epoll_event.events = EPOLLIN;
	epoll_event.data.fd = fd;
	ret = epoll_ctl(pfd, EPOLL_CTL_ADD, fd, &epoll_event);
	if (ret) {
		perror("epoll_ctl");
		fd = -1;
		goto exit;
	}

	DUMPFOOTER = 0;
	DUMPADDR = 1;
	for (;;) {
		ssize_t s;
		char buf[BUFSIZE];
		__memset(buf, 0);
		ret = epoll_wait(pfd, &epoll_event, 1, options.timeout);
		if (ret < 0) {
			perror("epoll_wait");
			break;
		}
		else if (!ret) {
			fprintf(stderr, "%s: %s\n", "epoll_wait", strerror(ETIME));
			ret = -1;
			break;
		}

		s = read(fd, buf, sizeof(buf));
		if (s < 0) {
			perror("read");
			ret = s;
			break;
		}
		else if (!s) {
			break;
		}
		buf[s] = 0;
		hexdump(size, buf, s);
		size += s;

		if (size)
			DUMPHEADER = 0;
	}

	if (size)
		printf("%08x\n", size);

exit:
	if (fd > -1) {
		epoll_ctl(pfd, EPOLL_CTL_DEL, fd, &epoll_event);
	}

	if (pfd) {
		close(pfd);
		pfd = -1;
	}

	return !ret ? EXIT_SUCCESS : EXIT_FAILURE;
}
