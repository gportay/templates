/*
 * Copyright (c) 2014, 2020 Gaël PORTAY <gael.portay@gmail.com>.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation,
 * advertising materials, and other materials related to such
 * distribution and use acknowledge that the software was developed
 * by the <organization>. The name of the
 * <organization> may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
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
#include <sys/reboot.h>
#include <linux/input.h>

#define __strcmp(s1, s2) strncmp(s1, s2, sizeof(s1))
#define __sprintf(s, fmt, ...) snprintf(s, sizeof(s), fmt, __VA_ARGS__)
#define __perror(s, e) fprintf(stderr, "%s: %s\n", s, strerror(e))
#define __close(fd) do { \
        int __error = errno; \
        if (close(fd) == -1) \
                perror("close"); \
        errno = __error; \
} while(0)

static int VERBOSE = 0;
#define verbose(fmt, ...) if (VERBOSE) fprintf(stdout, fmt, ##__VA_ARGS__)

struct options {
	int force;
	int timeout;
	const char *script;
	const char *device;
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
	fprintf(f, "Usage: %s [OPTIONS] DEVICE\n\n"
		   "Options:\n"
		   " -t or --timeout SECOND Set time-out in seconds [default=-1].\n"
		   " -s or --script         Set script [default=/usr/share/reboot].\n"
		   " -v or --verbose        Turn on verbose messages.\n"
		   " -V or --version        Display the version.\n"
		   " -h or --help           Display this message.\n"
		   "", applet(arg0));
}

int parse_arguments(struct options *opts, int argc, char * const argv[])
{
	static const struct option long_options[] = {
		{ "timeout", required_argument, NULL, 't' },
		{ "script",  required_argument, NULL, 's' },
		{ "verbose", no_argument,       NULL, 'v' },
		{ "version", no_argument,       NULL, 'V' },
		{ "help",    no_argument,       NULL, 'h' },
		{ NULL,      no_argument,       NULL, 0   }
	};

	opts->timeout = -1;
	opts->script = "/usr/share/input-event.action";
	opterr = 1;
	for (;;) {
		int index;
		int c = getopt_long(argc, argv, "t:vVh", long_options, &index);
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

		case 's':
			opts->script = optarg;
			break;

		case 'v':
			VERBOSE++;
			break;

		case 'V':
			printf("%s\n", VERSION);
			exit(EXIT_SUCCESS);
			break;

		case 'h':
			usage(stdout, argv[0]);
			exit(EXIT_SUCCESS);
			break;

		default:
		case '?':
			return -1;
		}
	}

	return optind;
}

int main(int argc, char * const argv[])
{
	int pfd = -1, fd = -1, ret = -1;
	struct epoll_event epoll_event;
	struct options options = {
		.timeout = -1,
		.device = NULL,
	};

	int argi = parse_arguments(&options, argc, argv);
	if (argi < 0) {
		fprintf(stderr, "Error: %s: Invalid option!\n", argv[optind-1]);
		exit(EXIT_FAILURE);
	} else if (argc - argi <= 0) {
		usage(stdout, argv[0]);
		fprintf(stderr, "Error: Too few arguments!\n");
		exit(EXIT_FAILURE);
	} else if (argc - argi > 1) {
		usage(stdout, argv[0]);
		fprintf(stderr, "Error: Too many arguments!\n");
		exit(EXIT_FAILURE);
	}
	options.device = argv[argi++];

	pfd = epoll_create(1);
	if (pfd < 0) {
		perror("epoll_create");
		exit(EXIT_FAILURE);
	}

	fd = open(options.device, O_RDONLY, 0);
	if (fd < 0) {
		perror("open");
		goto exit;
	}

	epoll_event.events = EPOLLIN;
	epoll_event.data.fd = fd;
	ret = epoll_ctl(pfd, EPOLL_CTL_ADD, fd, &epoll_event);
	if (ret) {
		perror("epoll_ctl");
		goto exit;
	}

	for (;;) {
		struct input_event input_event;
		char buf[BUFSIZ];
		ret = epoll_wait(pfd, &epoll_event, 1, options.timeout);
		if (ret < 0) {
			perror("epoll_wait");
			break;
		} else if (!ret) {
			__perror("epoll_wait", ETIME);
			break;
		}

		ret = read(fd, (char *) &input_event, sizeof(input_event));
		if (ret < 0) {
			perror("read");
			break;
		} else if (!ret) {
			break;
		}

		verbose("code: %u, value: %u\n", input_event.code, input_event.value);
		if (!input_event.code)
			continue;

		__sprintf(buf, "DEVNAME=%s %s %u %u", options.device, options.script, input_event.code, input_event.value);
		if (system(buf))
			perror("system");
	}

	ret = 0;
exit:
	if (fd > -1) {
		epoll_ctl(pfd, EPOLL_CTL_DEL, fd, &epoll_event);
		__close(fd);
		fd = -1;
	}

	if (pfd) {
		__close(pfd);
		pfd = -1;
	}

	return !ret ? EXIT_SUCCESS : EXIT_FAILURE;
}
