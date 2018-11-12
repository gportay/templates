/*
 *  Copyright (C) 2018 Savoir-Faire Linux Inc.
 *
 *  Authors:
 *       Gaël PORTAY <gael.portay@savoirfairelinux.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#else
const char VERSION[] = __DATE__ " " __TIME__;
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <getopt.h>
#include <signal.h>

struct options {
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
	fprintf(f, "Usage: %s [OPTIONS]\n"
		   "\n"
		   "sigwaitinfo(1) is a good template to begin with a new CLI\n"
		   "in C, based on a basic skeleton using sigwaitinfo(3p) as\n"
		   "main loop.\n"
		   "\n"
		   "Options:\n"
		   " -h or --help          Display this message.\n"
		   " -V or --version       Display the version.\n"
		   "\n"
		   "Signals:\n"
		   "SIGTERM                Terminate.\n"
		   "", applet(arg0));
}

int parse_arguments(struct options *opts, int argc, char * const argv[])
{
	static const struct option long_options[] = {
		{ "version", no_argument,       NULL, 'V' },
		{ "help",    no_argument,       NULL, 'h' },
		{ NULL,      no_argument,       NULL, 0   }
	};

	opterr = 0;
	opts->argc = 0;
	opts->argv = NULL;
	for (;;) {
		int index;
		int c = getopt_long(argc, argv, "Vh", long_options, &index);
		if (c == -1) {
			break;
		}

		switch (c) {
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

	opts->argc = argc;
	opts->argv = argv;
	return optind;
}

int main(int argc, char * const argv[])
{
	int sig, argi, ret = EXIT_FAILURE;
	static struct options options;
	static sigset_t sigset;

	argi = parse_arguments(&options, argc, argv);
	if (argi < 0) {
		fprintf(stderr, "Error: %s: Invalid option!\n", argv[optind-1]);
		exit(EXIT_FAILURE);
	} else if (argc - argi > 0) {
		usage(stdout, argv[0]);
		fprintf(stderr, "Error: Too many arguments!\n");
		exit(EXIT_FAILURE);
	}

	if (sigemptyset(&sigset) == -1) {
		perror("sigemptyset");
		exit(EXIT_FAILURE);
	}

	sig = SIGTERM;
	if (sigaddset(&sigset, sig) == -1) {
		perror("sigaddset");
		exit(EXIT_FAILURE);
	}

	if (sigprocmask(SIG_SETMASK, &sigset, NULL) == -1) {
		perror("sigprocmask");
		exit(EXIT_FAILURE);
	}

	for (;;) {
		siginfo_t siginfo;
		sig = sigwaitinfo(&sigset, &siginfo);
		if (sig == -1) {
			if (errno == EINTR || errno == EAGAIN)
				continue;

			perror("sigwaitinfo");
			break;
		}

		if (sig == SIGTERM) {
			ret = EXIT_SUCCESS;
			break;
		}

		fprintf(stderr, "%s: Uncaught signal!\n", strsignal(sig));
		break;
	}

	if (sigprocmask(SIG_UNBLOCK, &sigset, NULL) == -1)
		perror("sigprocmask");

	return ret;
}
