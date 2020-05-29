/*
 * Copyright (c) 2014 Gaël PORTAY <gael.portay@gmail.com>.
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <sys/epoll.h>
#include <sys/reboot.h>
#include <linux/input.h>

#define BUFSIZE 256
#define __strcmp(s1, s2) strncmp(s1, s2, sizeof(s1))
#define __sprintf(s, fmt, ...) snprintf(s, sizeof(s), fmt, __VA_ARGS__)

const char VERSION[] = "1.0";
static int VERBOSE = 0;

struct options_t {
	int force;
	int timeout;
	const char *script;
	const char *device;
};

const char *applet(const char * arg0)
{
	const char *applet = strrchr(arg0, '/');
	if (!applet)
		applet = arg0;
	else
		applet++;

	return applet;
}

void usage(FILE * f, char * const arg0)
{
  fprintf(f, "Usage: %s [OPTIONS] DEVICE\n\n"
	     "Options:\n"
	     " -t or --timeout SECOND Set time-out in seconds [default=-1].\n"
	     " -s or --script         Set script [default=/usr/share/reboot].\n"
	     " -v or --verbose        Turn on verbosity.\n"
	     " -V or --version        Display the version.\n"
	     " -h or --help           Display this message.\n"
	     "", applet(arg0));
}

int parse_arguments(struct options_t *opts, int argc, char * const argv[])
{
	static const struct option lopts[] = {
		{ "timeout", required_argument, NULL, 't' },
		{ "script",  required_argument, NULL, 's' },
		{ "verbose", no_argument,       NULL, 'v' },
		{ "version", no_argument,       NULL, 'V' },
		{ "help",    no_argument,       NULL, 'h' },
		{ NULL,      no_argument,       NULL, 0   }
	};

	opts->script = "/usr/share/input-event.action";

	for (;;) {
		int i, c = getopt_long(argc, argv, "t:vVh", lopts, &i);
		if (c == -1)
			break;

		switch (c) {
		case 't': {
			char *e;
			long l = strtol(optarg, &e, 0);
			if (*e) {
				fprintf(stderr, "Invalid time-out argument <%s>!\n", optarg);
				exit(EXIT_FAILURE);
			}
			opts->timeout = l;
			break;
		}

		case 's':
			opts->script = optarg;
			break;

		case 'v':
			VERBOSE = 1;
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
			printf("?? getopt returned character code 0%o ??\n", c);
			exit(EXIT_FAILURE);
		}
	}

	return optind;
}

int main(int argc, char * const argv[]) {
	int pfd = -1;
	int fd = -1;
	int ret = EXIT_FAILURE;
	struct epoll_event epoll_event;
	struct input_event input_event;
	struct options_t options = {
		.timeout = -1,
		.device = NULL,
	};

	int argi = parse_arguments(&options, argc, argv);
	if (argi < 0)
		exit(EXIT_FAILURE);
	else if (argc - argi <= 0) {
		usage(stdout, argv[0]);
		fprintf(stderr, "Too few arguments!\n");
		exit(EXIT_FAILURE);
	}
	else if (argc - argi > 1) {
		usage(stdout, argv[0]);
		fprintf(stderr, "Too many arguments!\n");
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
	if (epoll_ctl(pfd, EPOLL_CTL_ADD, fd, &epoll_event)) {
		perror("epoll_ctl");
		goto exit;
	}

	for (;;) {
		char buf[BUFSIZE];
		ret = epoll_wait(pfd, &epoll_event, 1, options.timeout);
		if (ret < 0) {
			perror("epoll_wait");
			break;
		}

		if (ret == 0) {
			ret = EXIT_SUCCESS;
			break;
		}

		ret = read(fd, (char *) &input_event, sizeof(input_event));
		if (ret < 0) {
			perror("read");
			break;
		}

		if (!input_event.code)
			continue;

		__sprintf(buf, "DEVNAME=%s %s %u %u", options.device, options.script, input_event.code, input_event.value);
		if (system(buf))
			perror("system");
	}

exit:
	if (fd) {
		epoll_ctl(pfd, EPOLL_CTL_DEL, fd, &epoll_event);
		close(fd);
		fd = -1;
	}

	if (pfd) {
		close(pfd);
		pfd = -1;
	}

	return ret;
}
