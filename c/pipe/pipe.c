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
	int status;
	int fd[2];
	pid_t pid;

	(void)argc;

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
			ssize_t s = read(fd[0], buf, sizeof(buf));
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
	_exit(127);
}

int main(int argc, char *argv[])
{
	int i;

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
