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
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <asm/types.h>
#include <linux/netlink.h>

static int VERBOSE = 0;
static int DEBUG = 0;
#define verbose(fmt, ...) if (VERBOSE) fprintf(stderr, fmt, ##__VA_ARGS__)
#define debug(fmt, ...) if (DEBUG) fprintf(stderr, fmt, ##__VA_ARGS__)
#define __memcmp(s1, s2) memcmp(s1, s2, sizeof(s2) - 1)
#define __close(fd) do { \
	int __error = errno; \
	if (close(fd) == -1) \
		perror("close"); \
	errno = __error; \
} while(0)

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

int daemonize(const char *path, char * const argv[], const char *devname)
{
	pid_t pid = fork();
	if (pid == -1) {
		perror("fork");
		return -1;
	}

	/* Parent */
	if (pid) {
		int status;

		if (waitpid(pid, &status, 0) == -1) {
			perror("waitpid");
			return -1;
		}

		if (WIFEXITED(status))
			return WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			printf("%s\n", strsignal(WTERMSIG(status)));

		return status;
	}

	/* Child */
	pid = fork();
	if (pid == -1) {
		perror("fork");
		exit(EXIT_FAILURE);
	} else if (pid) {
		exit(EXIT_SUCCESS);
	}

	/* Daemon */
	if (devname) {
		int fd;

		close(STDIN_FILENO);
		fd = open(devname, O_RDONLY|O_NOCTTY);
		if (fd == -1)
			perror("open");

		close(STDOUT_FILENO);
		fd = open(devname, O_WRONLY|O_NOCTTY);
		if (fd == -1)
			perror("open");

		close(STDERR_FILENO);
		dup2(STDOUT_FILENO, STDERR_FILENO);
	}

	execv(path, argv);
	_exit(127);
}

int setup_signal(int fd, int signal)
{
	int flags;

	if (fcntl(fd, F_SETSIG, signal) == -1) {
		perror("fcntl (F_SETSIG):");
		return -1;
	}

	if (fcntl(fd, F_SETOWN, getpid()) == -1) {
		perror("fcntl (F_SETOWN):");
		return -1;
	}

	flags = fcntl(fd, F_GETFL);
	if (flags == -1) {
		perror("fcntl (F_GETFL):");
		return -1;
	}

	if (fcntl(fd, F_SETFL, flags | O_ASYNC | O_NONBLOCK) == -1) {
		perror("fcntl (F_SETFL):");
		return -1;
	}

	return 0;
}

int netlink_open(struct sockaddr_nl *addr, int signal)
{
	int fd;

	assert(addr);
	memset(addr, 0, sizeof(*addr));
	addr->nl_family = AF_NETLINK;
	addr->nl_pid = getpid();
	addr->nl_groups = NETLINK_KOBJECT_UEVENT;

	fd = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_KOBJECT_UEVENT);
	if (fd == -1) {
		perror("socket");
		return -1;
	}

	if (bind(fd, (struct sockaddr *)addr, sizeof(*addr)) == -1) {
		perror("bind");
		goto error;
	}

	if (setup_signal(fd, signal) == -1) {
		__close(fd);
		goto error;
	}

	return fd;

error:
	__close(fd);
	return -1;
}

ssize_t netlink_recv(int fd, struct sockaddr_nl *addr)
{
	char buf[UEVENT_BUFFER_SIZE];
	struct iovec iov = {
		.iov_base = buf,
		.iov_len = sizeof(buf),
	};
	struct msghdr msg = {
		.msg_name = addr,
		.msg_namelen = sizeof(*addr),
		.msg_iov = &iov,
		.msg_iovlen = 1,
		.msg_control = NULL,
		.msg_controllen = 0,
		.msg_flags = 0,
	};
	ssize_t len;

	for (;;) {
		char *n, *s;
		ssize_t l;

		l = recvmsg(fd, &msg, 0);
		if (l == -1) {
			if (errno != EAGAIN) {
				perror("recvmsg");
				break;
			}

			break;
		} else if (!l) {
			break;
		}

		s = buf;
		buf[l] = 0;
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
			if (!n || n == s)
				break;

			printf("%s\n", s);
			s = n + 1;
		}

		printf("\n");
		len += l;
	}

	return len;
}

int main(int argc, char * const argv[])
{
	int fd, sig, ret = EXIT_FAILURE;
	static struct options_t options;
	struct sockaddr_nl addr;
	static sigset_t sigset;

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

	sig = SIGCHLD;
	if (sigaddset(&sigset, sig) == -1) {
		perror("sigaddset");
		return EXIT_FAILURE;
	}

	sig = SIGIO;
	if (sigaddset(&sigset, sig) == -1) {
		perror("sigaddset");
		return EXIT_FAILURE;
	}

	if (sigprocmask(SIG_SETMASK, &sigset, NULL) == -1) {
		perror("sigprocmask");
		return EXIT_FAILURE;
	}

	if (sigprocmask(SIG_SETMASK, &sigset, NULL) == -1) {
		perror("perror");
		return EXIT_FAILURE;
	}

	char *rcS[] = { "/etc/init.d/rcS", "start", NULL };
	daemonize("/etc/init.d/rcS", rcS, NULL);

	char *sh[] = { "-/bin/sh", NULL };
	daemonize("/bin/sh", sh, NULL);

	fd = netlink_open(&addr, SIGIO);
	if (fd == -1) {
		fprintf(stderr, "Oops!\n");
		return EXIT_FAILURE;
	}

	printf("tini started!\n");

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

		if (sig == SIGCHLD) {
			while (waitpid(-1, NULL, 0) > 0);
			continue;
		}

		if (sig == SIGIO) {
			netlink_recv(fd, &addr);
			continue;
		}

		if ((sig == SIGTERM) || (sig == SIGINT) || \
		    (sig == SIGUSR1) || (sig == SIGUSR2)) {
			ret = EXIT_SUCCESS;
			break;
		}
	}

	if (sig == SIGUSR1) {
		execv(argv[0], argv);
		exit(EXIT_FAILURE);
	}

	if (sigprocmask(SIG_UNBLOCK, &sigset, NULL) == -1)
		perror("sigprocmask");

	while (waitpid(-1, NULL, 0) > 0);

	return ret;
}
