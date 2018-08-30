/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2017-2018 Gaël PORTAY <gael.portay@savoirfairelinux.com>
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
#include <limits.h>
#include <errno.h>
#include <getopt.h>
#include <assert.h>

#include <sys/epoll.h>

#include <czmq.h>

#include "zpc-message.pb-c.h"

#include "hexdump.h"

static int DEBUG = 0;
static int VERBOSE = 0;
#define verbose(fmt, ...) if (VERBOSE) fprintf(stdout, fmt, ##__VA_ARGS__)
#define debug(fmt, ...) if (DEBUG) fprintf(stderr, fmt, ##__VA_ARGS__)
#define hexdebug(addr, buf, size) if (DEBUG) fhexdump(stderr, addr, buf, size)

#define __strncmp(s1, s2) strncmp(s1, s2, sizeof(s2))

struct options {
	int timeout;
};

static inline const char *applet(const char *arg0)
{
	char *s = strrchr(arg0, '/');
	if (!s)
		return arg0;

	return s+1;
}

static inline void _perror(const char *s, int error)
{
	fprintf(stderr, "%s: %s\n", s, strerror(error));
}

static inline void zmq_perror(const char *s)
{
	fprintf(stderr, "%s: %s\n", s, zmq_strerror(zmq_errno()));
}

void usage(FILE * f, char * const arg0)
{
	fprintf(f, "Usage: %s [OPTIONS] ENDPOINT TOPIC [FILE=STDIN]\n\n"
		   "Options:\n"
		   " -t or --timeout MS Set timeout [default=-1].\n"
		   " -v or --verbose    Turn on verbose messages.\n"
		   " -D or --debug      Turn on debug messages.\n"
		   " -V or --version    Display the version.\n"
		   " -h or --help       Display this message.\n"
		   "", applet(arg0));
}

int publish_wait(int timeout, const char *endpoint, const char *publish,
		 const char *file)
{
	int ret = 1;
	int pfd = -1;
	int fd = -1;
	struct epoll_event epoll_event;
	zsock_t *pub = NULL;

	pfd = epoll_create(1);
	if (pfd < 0) {
		perror("epoll_create");
		goto exit;
	}

	if (*file && __strncmp(file, "-")) {
		verbose("Opening %s...\n", file);
		fd = open(file, O_RDONLY);
		if (fd == -1) {
			perror("open");
			goto exit;
		}
	} else {
		fd = STDIN_FILENO;
	}
	epoll_event.events = EPOLLIN;
	epoll_event.data.fd = fd;
	if (epoll_ctl(pfd, EPOLL_CTL_ADD, fd, &epoll_event)) {
		perror("epoll_ctl");
		goto exit;
	}

	verbose("Binding to %s...\n", endpoint);
	pub = zsock_new_pub(endpoint);
	if (!pub) {
		zmq_perror("zsock_new_pub");
		goto exit;
	}

	for (;;) {
		char buf[BUFSIZ];
		ssize_t size;
		size_t len;
		uint8_t *data;
		Message msg = MESSAGE__INIT;

		ret = epoll_wait(pfd, &epoll_event, 1, timeout);
		if (ret < 0) {
			perror("epoll_wait");
			break;
		} else if (!ret) {
			verbose("Expired!\n");
			_perror("epoll_wait", ETIME);
			continue;
		}

		assert(epoll_event.data.fd == fd);
		size = read(fd, buf, sizeof(buf));
		if (size < 0) {
			perror("read");
			break;
		} else if (!size) {
			ret = 0;
			break;
		}
		buf[size] = 0;

		msg.data = buf;
		len = message__get_packed_size(&msg);
		data = malloc(len);
		if (!data) {
			perror("malloc");
			break;
		}

		message__pack(&msg, data);
		hexdebug(0, data, len);

		verbose("Sending...\n");
		if (zsock_send(pub, "sb", publish, data, len) != 0) {
			zmq_perror("zsock_send");
			break;
		}

		verbose("%s/%s: ", endpoint, publish);
		printf("%s", buf);
	}

	if (epoll_ctl(pfd, EPOLL_CTL_DEL, fd, &epoll_event)) {
		perror("epoll_ctl");
		goto exit;
	}

	zsock_flush(pub);
	sleep(2);
exit:
	if (pub) {
		zsock_destroy(&pub);
	}

	if (fd != -1) {
		close(fd);
	}

	if (pfd != -1) {
		close(pfd);
	}

	return ret;
}

int parse_arguments(struct options *opts, int argc, char * const argv[])
{
	static const struct option long_options[] = {
		{ "timeout", required_argument, NULL, 't' },
		{ "verbose", no_argument,       NULL, 'v' },
		{ "debug",   no_argument,       NULL, 'D' },
		{ "version", no_argument,       NULL, 'V' },
		{ "help",    no_argument,       NULL, 'h' },
		{ NULL,      no_argument,       NULL, 0   }
	};

	opts->timeout = -1;
	opterr = 1;
	for (;;) {
		int index;
		int c = getopt_long(argc, argv, "t:vDVh", long_options, &index);
		if (c == -1) {
			break;
		}

		switch (c) {
		case 't': {
			char *e;
			unsigned long ul = strtoul(optarg, &e, 0);
			if (*e || (errno == ERANGE && ul == ULONG_MAX)) {
				fprintf(stderr, "Error: "
					"Invalid timeout argument \"%s\"!\n",
					*e ? e : optarg);
				exit(EXIT_FAILURE);
			}
			opts->timeout = ul;
			break;
		}

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
		}
	}

	return optind;
}

int main(int argc, char * const argv[])
{
	struct options options;

	int argi = parse_arguments(&options, argc, argv);
	if (argi < 0) {
		fprintf(stderr, "Error: Invalid argument!\n");
		exit(EXIT_FAILURE);
	} else if (argc - argi < 2) {
		usage(stdout, argv[0]);
		fprintf(stderr, "Error: Too few arguments!\n");
		exit(EXIT_FAILURE);
	} else if (argc - argi > 3) {
		usage(stdout, argv[0]);
		fprintf(stderr, "Error: Too many arguments!\n");
		exit(EXIT_FAILURE);
	}

	debug("timeout: %i\n", options.timeout);
	debug("endpoint: %s\n", argv[optind]);
	debug("publish: %s\n", argv[optind+1]);
	debug("file: %s\n", (argc - argi == 3) ? argv[optind+2] : "-");

	return publish_wait(options.timeout, argv[optind], argv[optind+1],
			    (argc - argi == 3) ? argv[optind+2] : "-") == 0;
}
