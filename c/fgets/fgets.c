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

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>

void sighandler(int signum)
{
	fprintf(stderr, "%s\n", sys_siglist[signum]);
}

int main()
{
	char buf[BUFSIZ];

	struct sigaction action;
	action.sa_handler = sighandler;
	action.sa_flags = 0;
	sigemptyset(&action.sa_mask);

	if (sigaction(SIGINT, &action, NULL) == -1) {
		perror("sigaction");
		return EXIT_FAILURE;
	}

	if (sigaction(SIGTERM, &action, NULL) == -1) {
		perror("sigaction");
		return EXIT_FAILURE;
	}

	if (sigaction(SIGQUIT, &action, NULL) == -1) {
		perror("sigaction");
		return EXIT_FAILURE;
	}

for (;;) {
	errno = EAGAIN;
	char *s = fgets(buf, sizeof(buf), stdin);
	printf("s: %s\n", s);
	printf("feof(stdin): %i\n", feof(stdin));
	if (!s) {
		if (errno) {
			perror("fgets");
			return EXIT_FAILURE;
		}
		buf[0] = 0;
		break;
	}
}

	printf("%s\n", buf);

	return EXIT_SUCCESS;
}
