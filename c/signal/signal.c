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
#include <errno.h>
#include <getopt.h>
#include <signal.h>
#include <sys/wait.h>

static int VERBOSE = 0;
static int DEBUG = 0;
#define verbose(fmt, ...) if (VERBOSE) fprintf(stderr, fmt, ##__VA_ARGS__)
#define debug(fmt, ...) if (DEBUG) fprintf(stderr, fmt, ##__VA_ARGS__)

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
	static sigset_t sigset;
	int sig, ret = EXIT_FAILURE;

	int argi = parse_arguments(&options, argc, argv);
	if (argi < 0) {
		fprintf(stderr, "Error: Invalid argument!\n");
		exit(EXIT_FAILURE);
	} else if (argc - argi >= 1) {
		usage(stdout, argv[0]);
		fprintf(stderr, "Error: Too many arguments!\n");
		exit(EXIT_FAILURE);
	}

	if (sigemptyset(&sigset) == -1) {
		perror("sigemptyset");
		return EXIT_FAILURE;
	}

	sig = SIGTERM;
	if (sigaddset(&sigset, sig) == -1) {
		perror("sigaddset");
		return EXIT_FAILURE;
	}

	sig = SIGINT;
	if (sigaddset(&sigset, sig) == -1) {
		perror("sigaddset");
		return EXIT_FAILURE;
	}

	sig = SIGUSR1;
	if (sigaddset(&sigset, sig) == -1) {
		perror("sigaddset");
		return EXIT_FAILURE;
	}

	sig = SIGUSR2;
	if (sigaddset(&sigset, sig) == -1) {
		perror("sigaddset");
		return EXIT_FAILURE;
	}

	if (sigprocmask(SIG_SETMASK, &sigset, NULL) == -1) {
		perror("sigprocmask");
		return EXIT_FAILURE;
	}

	for (;;) {
		siginfo_t siginfo;
		pid_t pid = getpid();

		verbose("Kill me using using signals SIGINT (^C) or SIGTERM or any of the two user defined signals SIGUSR1 or SIGUSR2.\nHit Ctrl+C to raise SIGINT or try the command below in another terminal:\n\t$ kill -SIGTERM %i\n", pid);
		sig = sigwaitinfo(&sigset, &siginfo);
		if (sig == -1) {
			if (errno == EINTR)
				continue;

			perror("sigwaitinfo");
			break;
		}

		debug("sigwaitinfo(): %i: %s\n", sig, strsignal(sig));

		if ((sig == SIGTERM) || (sig == SIGINT)) {
			ret = EXIT_SUCCESS;
			break;
		}

		if ((sig == SIGUSR1) || (sig == SIGUSR2)) {
			printf("%s\n", strsignal(sig));
		}
	}

	if (sigprocmask(SIG_UNBLOCK, &sigset, NULL) == -1) {
		perror("sigprocmask");
	}

	return ret;
}
