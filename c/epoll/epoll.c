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

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/epoll.h>

#define BUFSIZE 256

static int DEBUG = 0;
#define debug(fmt, ...) if (DEBUG) fprintf(stderr, fmt, ##__VA_ARGS__)

struct options_t {
	int timeout;
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
	fprintf(f, "Usage: %s [OPTIONS]\n\n"
		   "Options:\n"
		   " -t or --timeout SECOND Set time-out in seconds [default=-1].\n"
		   " -D or --debug          Turn on debug messages.\n"
		   " -h or --help           Display this message.\n"
		   " -V or --version        Display the version.\n"
		   "", applet(arg0));
}

int parse_arguments(struct options_t *opts, int argc, char * const argv[])
{
	static const struct option long_options[] = {
		{ "timeout", required_argument, NULL, 't' },
		{ "debug",   no_argument,       NULL, 'D' },
		{ "version", no_argument,       NULL, 'V' },
		{ "help",    no_argument,       NULL, 'h' },
		{ NULL,      no_argument,       NULL, 0   }
	};

	for (;;) {
		int option_index;
		int c = getopt_long(argc, argv, "t:DVh", long_options, &option_index);
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
	int pfd = -1;
	int fd = -1;
	int ret = EXIT_FAILURE;
	struct epoll_event epoll_event;
	struct options_t options = {
		.timeout = -1,
	};

	int argi = parse_arguments(&options, argc, argv);
	if (argi < 0) {
		fprintf(stderr, "Error: Invalid argument!\n");
		exit(EXIT_FAILURE);
	} else if (argc - argi >= 1) {
		usage(stdout, argv[0]);
		fprintf(stderr, "Error: Too many arguments!\n");
		exit(EXIT_FAILURE);
	}

	pfd = epoll_create(1);
	debug("epoll_create(size: 1) returned %i\n", pfd);
	if (pfd < 0) {
		perror("epoll_create");
		exit(EXIT_FAILURE);
	}

	fd = STDIN_FILENO;
	epoll_event.events = EPOLLIN;
	epoll_event.data.fd = fd;
	ret = epoll_ctl(pfd, EPOLL_CTL_ADD, fd, &epoll_event);
	debug("epoll_ctl(pfd: %i, op: 0x%x, fd: %i, evt: @%p { events: 0x%x, data.fd: %i }) returned %i\n", pfd, EPOLL_CTL_ADD, fd, &epoll_event, epoll_event.events, epoll_event.data.fd, ret);
	if (ret) {
		perror("epoll_ctl");
		fd = -1;
		goto exit;
	}

	for (;;) {
		char buf[BUFSIZE];
		ret = epoll_wait(pfd, &epoll_event, 1, options.timeout);
		debug("epoll_wait(pfd: %i, ...) returned %i\n", pfd, ret);
		if (ret < 0) {
			perror("epoll_wait");
			break;
		}
		else if (!ret) {
			fprintf(stderr, "%s: %s\n", "epoll_wait", strerror(ETIME));
			ret = -1;
			break;
		}

		ret = read(fd, buf, sizeof(buf));
		debug("read(fd: %i, ...) returned %i\n", fd, ret);
		if (ret < 0) {
			perror("read");
			break;
		}
		else if (!ret) {
			break;
		}
		buf[ret] = 0;
		printf("%s", buf);
	}

exit:
	if (fd > -1) {
		epoll_ctl(pfd, EPOLL_CTL_DEL, fd, &epoll_event);
		debug("epoll_ctl(pfd: %i, op: 0x%x, fd: %i, evt: @%p { events: 0x%x, data.fd: %i }) returned\n", pfd, EPOLL_CTL_DEL, fd, &epoll_event, epoll_event.events, epoll_event.data.fd);
	}

	if (pfd) {
		close(pfd);
		debug("close(fd: %i) returned\n", pfd);
		pfd = -1;
	}

	return !ret ? EXIT_SUCCESS : EXIT_FAILURE;
}
