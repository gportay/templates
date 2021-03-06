/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2017 Gaël PORTAY <gael.portay@savoirfairelinux.com>
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
#include <assert.h>
#include <sys/types.h>
#include <sys/wait.h>

static inline const char *applet(const char *arg0)
{
	char *s = strrchr(arg0, '/');
	if (!s)
		return arg0;

	return s+1;
}

void usage(FILE * f, char * const arg0)
{
	fprintf(f, "Usage: %s [OPTIONS] PATH [ARGV...]\n", applet(arg0));
}

int pipe_exec(int argc, char * const argv[])
{
	char buf[BUFSIZ];
	size_t s, size;
	int status;
	int fd[2];
	pid_t pid;

	if (pipe(fd) == -1) {
		perror("pipe");
		return -1;
	}

	pid = fork();
	if (pid == -1) {
		perror("fork");
		return -1;
	}

	/* Parent */
	if (pid) {
		close(fd[1]);
		for (;;) {
			s = read(fd[0], buf, sizeof(buf));
			if (s == -1) {
				perror("read");
				return -1;
			} else if (!s) {
				break;
			}

			buf[s] = 0;
			printf("%s", buf);
		}
		close(fd[0]);

		wait(&status);
		if (WIFEXITED(status))
			return WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			printf("%s\n", strsignal(WTERMSIG(status)));

		return status;
	}

	/* Child */
	close(fd[0]);
	execv(argv[0], argv);
	perror("execv");
	exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
	int i, ret = EXIT_FAILURE;

	if (argc < 1) {
		usage(stdout, argv[0]);
		fprintf(stderr, "Error: Too few arguments!\n");
		exit(EXIT_FAILURE);
	}

	for (i = 0; i < (argc - 1); i++)
		argv[i] = argv[i+1];
	argv[i] = NULL;

	return pipe_exec(argc-1, &argv[0]);
}
