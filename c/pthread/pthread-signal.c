/*
 *  Copyright (C) 2018 Gaël PORTAY
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

#define _GNU_SOURCE

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
#include <sys/types.h>
#include <sys/syscall.h>
#include <fcntl.h>
#include <pthread.h>

static int VERBOSE = 0;
static int DEBUG = 0;
#define verbose(fmt, ...) if (VERBOSE) fprintf(stderr, fmt, ##__VA_ARGS__)
#define debug(fmt, ...) if (DEBUG) fprintf(stderr, fmt, ##__VA_ARGS__)

static inline pid_t gettid()
{
	return syscall(SYS_gettid);
}

static int main_thread(int argc, char * const argv[])
{
	int sig, flags, ret = EXIT_FAILURE;
	static sigset_t sigset;

	if (VERBOSE) {
		int i;
		verbose("Thread has %i argument%c:", argc,
			argc == 0 ? ' ' : 's');
		for (i = 0; i < argc; i++)
			verbose(" \"%s\"", argv[i]);
		verbose("\n");
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

	sig = SIGIO;
	if (sigaddset(&sigset, sig) == -1) {
		perror("sigaddset");
		return EXIT_FAILURE;
	}

	if (pthread_sigmask(SIG_SETMASK, &sigset, NULL) == -1) {
		perror("pthread_sigmask");
		return EXIT_FAILURE;
	}

	if (fcntl(STDIN_FILENO, F_SETSIG, SIGIO) == -1) {
		perror("fcntl");
		return EXIT_FAILURE;
	}

	if (fcntl(STDIN_FILENO, F_SETOWN, getpid()) == -1) {
		perror("fcntl");
		return EXIT_FAILURE;
	}

	flags = fcntl(STDIN_FILENO, F_GETFL);
	if (flags == -1) {
		perror("fcntl");
		return EXIT_FAILURE;
	}

	flags |= (O_ASYNC | O_NONBLOCK | O_CLOEXEC);
	if (fcntl(STDIN_FILENO, F_SETFL, flags) == -1) {
		perror("fcntl");
		return EXIT_FAILURE;
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

		printf("%s\n", strsignal(sig));

		if (sig == SIGTERM) {
			ret = EXIT_SUCCESS;
			break;
		}

		if (sig == SIGUSR1) {
			printf("Cancel itself!\n");
			pthread_cancel(pthread_self());
			continue;
		}

		if (sig == SIGUSR2) {
			static int retval = 128 + SIGUSR2;
			printf("Exit %i itself!\n", retval);
			pthread_exit(&retval);
			continue;
		}

		if (sig == SIGIO) {
			for (;;) {
				char buf[BUFSIZ];
				ssize_t size;

				size = read(STDIN_FILENO, buf, sizeof(buf));
				if (size == -1) {
					perror("read");
					break;
				} else if (!size) {
					ret = EXIT_SUCCESS;
					break;
				}
				buf[size] = '\0';

				printf("%s", buf);
			}

			continue;
		}

		fprintf(stderr, "%s: Uncaught signal!\n", strsignal(sig));
		break;
	}

	if (pthread_sigmask(SIG_UNBLOCK, &sigset, NULL) == -1)
		perror("sigprocmask");

	return ret;
}

struct options {
	int argc;
	char * const *argv;
	struct timespec timeout;
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
	fprintf(f, "Usage: %s [OPTIONS] [THREAD_ARGS...]\n"
		   "\n"
		   "pthread-signal(1) is a good template to begin with a new \n"
		   "CLI in C, based on a basic skeleton using sigwaitinfo(3p)\n"
		   " as main loop in main thread that propagates signals such\n"
		   "as SIGTERM, SIGUSR1, SIGUSR2 to another thread.\n"
		   "\n"
		   "Options:\n"
		   " -t or --timeout SEC   Set timeout in sec [default=5].\n"
#ifdef _GNU_SOURCE
		   "                       Note: It is also used to wait for\n"
		   "                       thread to join before returning.\n"
#endif
		   " -v or --verbose       Turn on verbose messages.\n"
		   " -D or --debug         Turn on debug messages.\n"
		   " -h or --help          Display this message.\n"
		   " -V or --version       Display the version.\n"
		   "\n"
		   "Signals:\n"
		   "SIGINT                 Cancel thread.\n"
		   "SIGTERM                Signal thread using Terminate to\n"
		   "                       cause thread to exit.\n"
		   "SIGUSR1                Signal thread using User signal 1\n"
		   "                       to cause thread to cancel itself.\n"
		   "SIGUSR2                Signal thread using User signal 2\n"
		   "                       to cause thread to exit itself.\n"
		   "", applet(arg0));
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

	opterr = 0;
	opts->argc = 0;
	opts->argv = NULL;
	opts->timeout.tv_sec = 5;
	for (;;) {
		int index;
		int c = getopt_long(argc, argv, "t:vDVh", long_options, &index);
		if (c == -1) {
			break;
		}

		switch (c) {
		case 't': {
			char *e;
			long l = strtol(optarg, &e, 0);
			if (*e) {
				fprintf(stderr, "Error: %s: Invalid value!\n",
					optarg);
				exit(EXIT_FAILURE);
			}
			opts->timeout.tv_sec = l;
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

		default:
		case '?':
			return -1;
		}
	}

	opts->argc = argc;
	opts->argv = argv;
	return optind;
}

static void *start(void *arg)
{
	struct options *opts = (struct options *)arg;
	static int retval;

	verbose("[%i] Thread started!\n", gettid());
	retval = main_thread(opts->argc, opts->argv);
	verbose("[%i] Thread exited!\n", gettid());

	return &retval;
}

int main(int argc, char * const argv[])
{
	static struct options options, topts;
	int sig, argi, ret = EXIT_FAILURE;
	static sigset_t sigset;
	pthread_t t;
	void *tret;

	argi = parse_arguments(&options, argc, argv);
	if (argi < 0) {
		fprintf(stderr, "Error: %s: Invalid option!\n", argv[optind-1]);
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

	verbose("[%i] Creating thread...\n", gettid());
	topts.argc = argc - argi;
	topts.argv = &argv[argi];
	if (pthread_create(&t, NULL, start, &topts)) {
		perror("pthread_create");
		goto exit;
	}
	verbose("[%i] Thread created!\n", gettid());

	fprintf(stderr, "Kill me using using signals SIGINT (^C) or SIGTERM"
			" or any of the two user defined signals SIGUSR1 or"
			" SIGUSR2.\n"
			"Hit Ctrl+C to raise SIGINT or"
			" try the command below in another terminal:\n"
			"\t$ kill -SIGTERM %i\n", getpid());

	for (;;) {
		siginfo_t siginfo;
		sig = sigtimedwait(&sigset, &siginfo, &options.timeout);
		if (sig == -1) {
			if (errno == EINTR || errno == EAGAIN)
				continue;

			perror("sigtimedwait");
			break;
		}

		debug("sigtimedwait(): %i: %s\n", sig, strsignal(sig));

		if (sig == SIGINT) {
			verbose("[%i] Cancelling thread...\n", gettid());
			if (pthread_cancel(t)) {
				perror("pthread_cancel");
				break;
			}
			verbose("[%i] Thread cancelled!\n", gettid());

			break;
		}

		if ((sig == SIGTERM) || (sig == SIGUSR1) || (sig == SIGUSR2)) {
			verbose("[%i] Signaling thread using %i (%s)...\n",
			        gettid(), sig, strsignal(sig));
			if (pthread_kill(t, sig)) {
				perror("pthread_kill");
				break;
			}
			verbose("[%i] Thread signaled!\n", gettid());

			if (sig == SIGTERM)
				break;

			continue;
		}

		fprintf(stderr, "%s: Uncaught signal!\n", strsignal(sig));
		break;
	}

	if (sigprocmask(SIG_UNBLOCK, &sigset, NULL) == -1)
		perror("sigprocmask");

#ifdef _GNU_SOURCE
	if (options.timeout.tv_sec > 0) {
		struct timespec ts;
		if (clock_gettime(CLOCK_REALTIME, &ts) == 0) {
			verbose("[%i] Waiting thread to join (%lis)...\n",
				gettid(), options.timeout.tv_sec);
			ts.tv_sec += options.timeout.tv_sec;
			if (pthread_timedjoin_np(t, &tret, &ts)) {
				perror("pthread_timedjoin_np");
				goto exit;
			}
			verbose("[%i] Thread joined!\n", gettid());
		
			ret = EXIT_SUCCESS;
			goto exit;
		}
		perror("clock_gettime");
	}
#endif

	verbose("[%i] Waiting thread to join...\n", gettid());
	if (pthread_join(t, &tret)) {
		perror("pthread_join");
		goto exit;
	}
	verbose("[%i] Thread joined!\n", gettid());

	ret = EXIT_SUCCESS;

exit:
	if (tret == PTHREAD_CANCELED)
		fprintf(stderr, "Cancelled\n");
	else
		ret = *(int*)tret;

	return ret;
}
