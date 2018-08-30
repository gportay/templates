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
#include <limits.h>
#include <errno.h>
#include <getopt.h>
#include <assert.h>

#include <czmq.h>

#include "zpc-message.pb-c.h"

#include "hexdump.h"

static int DEBUG = 0;
static int VERBOSE = 0;
#define verbose(fmt, ...) if (VERBOSE) fprintf(stdout, fmt, ##__VA_ARGS__)
#define debug(fmt, ...) if (DEBUG) fprintf(stderr, fmt, ##__VA_ARGS__)
#define hexdebug(addr, buf, size) if (DEBUG) fhexdump(stderr, addr, buf, size)

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

static inline void zmq_perror(const char *s)
{
	fprintf(stderr, "%s: %s\n", s, zmq_strerror(zmq_errno()));
}

void usage(FILE * f, char * const arg0)
{
	fprintf(f, "Usage: %s [OPTIONS] ENDPOINT TOPIC\n\n"
		   "Options:\n"
		   " -t or --timeout MS Set timeout [default=-1].\n"
		   " -v or --verbose    Turn on verbose messages.\n"
		   " -D or --debug      Turn on debug messages.\n"
		   " -V or --version    Display the version.\n"
		   " -h or --help       Display this message.\n"
		   "", applet(arg0));
}

int subscribe_wait(int timeout, const char *endpoint, const char *subscribe)
{
	int ret = 1;
	zpoller_t *poller = NULL;
	zsock_t *sub = NULL;

	verbose("Connecting to %s/%s...\n", endpoint, subscribe);
	sub = zsock_new_sub(endpoint, subscribe);
	if (!sub) {
		zmq_perror("zsock_new_sub");
		goto exit;
	}

	poller = zpoller_new(sub, NULL);
	if (!poller) {
		zmq_perror("zpoller_new");
		goto exit;
	}

	for (;;) {
		char *publish;
		char *buf;
		size_t bufsize;
		zsock_t *sock;
		Message *msg;

		sock = (zsock_t *)zpoller_wait(poller, timeout);
		if (zpoller_terminated(poller)) {
			verbose("Terminated!\n");
			ret = 0;
			break;
		} else if (zpoller_expired(poller)) {
			verbose("Expired!\n");
			continue;
		}

		assert(sock == sub);
		verbose("Receiving...\n");
		if (zsock_recv(sub, "sb", &publish, &buf, &bufsize) != 0) {
			zmq_perror("zsock_recv");
			break;
		}

		assert(publish);
		assert(buf);
		msg = message__unpack(NULL, bufsize, buf);
		assert(msg);

		hexdebug(0, buf, bufsize);
		verbose("%s/%s: ", endpoint, publish);
		printf("%s", msg->data);

		free(publish);
		free(buf);
		message__free_unpacked(msg, NULL);
	}

	if (zpoller_remove(poller, sub)) {
		zmq_perror("zpoller_remote");
		goto exit;
	}

exit:
	if (sub) {
		zsock_destroy(&sub);
	}

	if (poller) {
		zpoller_destroy(&poller);
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
	} else if (argc - argi > 2) {
		usage(stdout, argv[0]);
		fprintf(stderr, "Error: Too many arguments!\n");
		exit(EXIT_FAILURE);
	}

	debug("timeout: %i\n", options.timeout);
	debug("endpoint: %s\n", argv[optind]);
	debug("subscribe: %s\n", argv[optind+1]);

	return subscribe_wait(options.timeout, argv[optind],
			      argv[optind+1]) == 0;
}
