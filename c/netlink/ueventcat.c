/*
 *  Copyright (C) 2017 Savoir-Faire Linux Inc.
 *
 *  Authors:
 *       Gaël PORTAY <gael.portay@savoirfairelinux.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
#include <getopt.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <asm/types.h>
#include <linux/netlink.h>

#include "hexdump.h"

static int VERBOSE = 0;
static int DEBUG = 0;
#define verbose(fmt, ...) if (VERBOSE) fprintf(stderr, fmt, ##__VA_ARGS__)
#define debug(fmt, ...) if (DEBUG) fprintf(stderr, fmt, ##__VA_ARGS__)
#define hexdebug(addr, buf, size) if (DEBUG) fhexdump(stderr, addr, buf, size)

#define __memcmp(s1, s2) memcmp(s1, s2, sizeof(s2) - 1)

#ifndef UEVENT_BUFFER_SIZE
#define UEVENT_BUFFER_SIZE 2048
#endif

#define LIBUDEV_STRING		"libudev"
#define LIBUDEV_MAGIC		0xfeedcafe
#define LIBUDEV_MAGIC_OFFSET	(sizeof(LIBUDEV_STRING))
#define LIBUDEV_PAYLOAD_OFFSET	(sizeof(LIBUDEV_STRING) + 2*sizeof(uint32_t))

struct options_t {
	int argc;
	char * const *argv;
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
		   "Concatenate UEVENTS(s) to standard output.\n\n"
		   "Read netlink socket.\n\n"
		   "Options:\n"
		   " -v or --verbose        Turn on verbose messages.\n"
		   " -D or --debug          Turn on debug messages.\n"
		   " -h or --help           Display this message.\n"
		   " -V or --version        Display the version.\n"
		   "", applet(arg0));
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

	opterr = 0;
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
		default:
			return -1;
		}
	}

	opts->argc = argc;
	opts->argv = argv;
	return optind;
}

int main(int argc, char * const argv[])
{
	static struct options_t options;
	char buf[UEVENT_BUFFER_SIZE];
	int fd, ret = EXIT_FAILURE;
	struct sockaddr_nl addr;
	struct iovec iov = {
		.iov_base = buf,
		.iov_len = sizeof(buf),
	};
	struct msghdr msg = {
		.msg_name = &addr,
		.msg_namelen = sizeof(addr),
		.msg_iov = &iov,
		.msg_iovlen = 1,
		.msg_control = NULL,
		.msg_controllen = 0,
		.msg_flags = 0,
	};

	int argi = parse_arguments(&options, argc, argv);
	if (argi < 0) {
		fprintf(stderr, "Error: %s: Invalid argument!\n",
				argv[optind-1]);
		exit(EXIT_FAILURE);
	} else if (argc - argi >= 1) {
		usage(stdout, argv[0]);
		fprintf(stderr, "Error: Too many arguments!\n");
		exit(EXIT_FAILURE);
	}

	memset(&addr, 0, sizeof(addr));
	addr.nl_family = AF_NETLINK;
	addr.nl_pid = getpid();
	addr.nl_groups = NETLINK_KOBJECT_UEVENT;

	fd = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_KOBJECT_UEVENT);
	if (fd == -1) {
		perror("socket");
		exit(EXIT_FAILURE);
	}

	if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
		perror("bind");
		exit(EXIT_FAILURE);
	}

	for (;;) {
		ssize_t len;
		char *n, *s;

		len = recvmsg(fd, &msg, 0);
		if (len == -1) {
			if (errno != EAGAIN) {
				perror("recvmsg");
				break;
			}

			continue;
		} else if (!len) {
			ret = EXIT_SUCCESS;
			break;
		}

		hexdebug(0, buf, len);
		s = buf;
		buf[len] = 0;
		if (!__memcmp(buf, "libudev")) {
			unsigned int *magic, *offset;

			magic = (unsigned int *)&buf[LIBUDEV_MAGIC_OFFSET];
			if (htonl(*magic) != LIBUDEV_MAGIC) {
				fprintf(stderr, "%x: Invalid magic!\n",
					htonl(*magic));
				continue;
			}

			offset = (unsigned int *)&buf[LIBUDEV_PAYLOAD_OFFSET];
			s += *offset;
		} else {
			s += strlen(s) + 1;
		}

		for (;;) {
			n = strchr(s, '\0');
			if (!n || n == s) {
				break;
			}

			printf("%s\n", s);

			s = n + 1;
		}

		printf("\n");
	}

	return ret;
}
