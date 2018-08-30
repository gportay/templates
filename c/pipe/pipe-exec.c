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

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <getopt.h>
#include <signal.h>
#include <sys/wait.h>

#include "pexec.h"

static int VERBOSE = 0;
static int DEBUG = 0;
#define verbose(fmt, ...) if (VERBOSE) fprintf(stderr, fmt, ##__VA_ARGS__)
#define debug(fmt, ...) if (DEBUG) fprintf(stderr, fmt, ##__VA_ARGS__)

int printpid(pid_t pid)
{
	char buf[256];
	snprintf(buf, sizeof(buf), "ls -l /proc/%i/fd", pid);
	return system(buf);
}

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
	fprintf(f, "Usage: %s [OPTIONS]\n\n"
		   "Options:\n"
		   " -v or --verbose        Turn on verbose messages.\n"
		   " -D or --debug          Turn on debug messages.\n"
		   " -h or --help           Display this message.\n"
		   " -V or --version        Display the version.\n"
		   "", applet(arg0));
}

int parse_arguments(struct options *opts, int argc, char * const argv[])
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

int read_execv(const char *path, char * const argv[])
{
	char buf[BUFSIZ];
	int status, fd;
	pid_t pid;
	ssize_t s;

	debug("pexecv()... ");
	fd = pexecv(&pid, path, argv);
	if (fd == -1)
		return -1;

	verbose("child: %i\n", pid);
	if (DEBUG > 1) {
		printf("\nParent:\n");
		printpid(getpid());
		printf("\nChild:\n");
		printpid(pid);
	}

	for (;;) {
		debug("read(fd: %i)... ", fd);
		s = read(fd, buf, sizeof(buf));
		if (s == -1) {
			perror("read");
			return -1;
		} else if (!s) {
			verbose("0\n");
			break;
		}

		buf[s] = 0;
		verbose("%zi: ", s);
		printf("%s", buf);
	}

	if (close(fd) == -1)
		perror("close");

	debug("waitpid(pid: %i)... ", pid);
	if (waitpid(pid, &status, 0) == -1) {
		perror("waitpid");
		return -1;
	}

	verbose("status: %i\n", status);
	if (WIFEXITED(status))
		return WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		printf("%s\n", strsignal(WTERMSIG(status)));

	return status;
}

int main(int argc, char * const argv[])
{
	static struct options options;
	int argi = parse_arguments(&options, argc, argv);
	if (argi < 0) {
		fprintf(stderr, "Error: %s: Invalid argument!\n",
				argv[optind-1]);
		exit(EXIT_FAILURE);
	} else if (argc - argi < 1) {
		usage(stdout, argv[0]);
		fprintf(stderr, "Error: Too few arguments!\n");
		exit(EXIT_FAILURE);
	}

	return read_execv(argv[argi], &argv[argi]);
}
