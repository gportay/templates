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

#ifdef HAVE_CONFIG_H
# include "config.h"
#else
const char VERSION[] = __DATE__ " " __TIME__;
#endif /* HAVE_CONFIG_H */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <signal.h>
#include <sys/syscall.h>
#include <dlfcn.h>
#include <pthread.h>

#include "plugin.h"
#include "strtonargv.h"

static int VERBOSE = 0;
static int DEBUG = 0;
#define verbose(fmt, ...) if (VERBOSE) fprintf(stderr, fmt, ##__VA_ARGS__)
#define debug(fmt, ...) if (DEBUG) fprintf(stderr, fmt, ##__VA_ARGS__)

static inline pid_t gettid()
{
	return syscall(SYS_gettid);
}

struct plugin_data {
	char buffer[BUFSIZ];
	int argc;
	char *argv[127];
	void *handler;
	pthread_t thread;
	int exited;
	int retval;
	struct plugin *plugin;
};

struct priv {
	int plugins_count;
	struct plugin_data *plugins_data;
	pthread_t main_thread;
};

static struct priv PRIVATE;

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
	fprintf(f, "Usage: %s [OPTIONS] PLUGIN...\n"
		   "\n"
		   "Run plugin entrypoints.\n"
		   "\n"
		   "Options:\n"
		   " -v or --verbose       Turn on verbose messages.\n"
		   " -D or --debug         Turn on debug messages.\n"
		   " -h or --help          Display this message.\n"
		   " -V or --version       Display the version.\n"
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
	opts->argc = 0;
	opts->argv = NULL;
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

		default:
		case '?':
			return -1;
		}
	}

	opts->argc = argc;
	opts->argv = argv;
	return optind;
}

static void exit_handler(void *arg)
{
	int i;
	(void)arg;

	for (i = 0; i < PRIVATE.plugins_count; i ++)
		if (!PRIVATE.plugins_data[i].exited)
			return;

	verbose("Raising %s!\n", strsignal(SIGTERM));
	if (pthread_kill(PRIVATE.main_thread, SIGTERM))
		perror("pthread_kill");
}

static void stop_handler(void *arg)
{
	struct plugin_data *data = (struct plugin_data *)arg;
	data->exited = 1;
}

static void *start_thread(void *arg)
{
	struct plugin_data *data = (struct plugin_data *)arg;

	pthread_cleanup_push(exit_handler, &PRIVATE);
	pthread_cleanup_push(stop_handler, arg);

	verbose("[%i] Thread started!\n", gettid());
	if (data->plugin->entrypoint) {
		data->retval = data->plugin->entrypoint(data->argc, data->argv);
	}
	verbose("[%i] Thread exited!\n", gettid());

	pthread_cleanup_pop(1);
	pthread_cleanup_pop(1);

	return &data->retval;
}

static int start_plugins(struct plugin_data data[], int argc, char * const argv[])
{
	int i, ret = EXIT_FAILURE;

	CFS = ":";
	for (i = 0; i < argc; i++) {
		struct plugin *sym;
		void *hdl;

		strncpy(data[i].buffer, argv[i], sizeof(data[i].buffer));
		data[i].argc = 127;
		strtonargv(data[i].argv, data[i].buffer, &data[i].argc);

		verbose("Opening library %s... ", data[i].argv[0]);
		hdl = dlopen(data[i].argv[0], RTLD_LAZY);
		if (!hdl) {
			fprintf(stderr, "%s\n", dlerror());
			goto exit;
		}
		verbose("done\n");

		verbose("Getting plugin symbol... ");
		sym = dlsym(hdl, "plugin");
		if (!sym) {
			fprintf(stderr, "%s\n", dlerror());
			goto exit;
		}
		verbose("done\n");

		verbose("Running plugin symbol... ");
		data[i].handler = hdl;
		data[i].plugin = sym;
		if (pthread_create(&data[i].thread, NULL, start_thread,
				   &data[i])) {
			perror("pthread_create");
			goto exit;
		}
		verbose("done\n");
	}

	ret = EXIT_SUCCESS;

exit:
	return ret;
}

static void stop_plugins(struct plugin_data data[], int argc, char * const argv[])
{
	int i;
	(void)argv;

	for (i = argc-1; i >= 0; i--) {
		void *hdl = data[i].handler;

		if (!hdl)
			continue;

		if (!data[i].exited) {
			void *retval;

			verbose("[%i] Cancelling thread...\n", gettid());
			if (pthread_cancel(data[i].thread))
				perror("pthread_cancel");
			else
				verbose("[%i] Thread cancelled!\n", gettid());

			verbose("[%i] Waiting thread to join...\n", gettid());
			if (pthread_join(data[i].thread, &retval))
				perror("pthread_join");
			else
				verbose("[%i] Thread joined!\n", gettid());

			data[i].retval = *(int *)retval;
		}

		if ((void *)&data[i].retval == PTHREAD_CANCELED)
			fprintf(stderr, "Cancelled\n");
		else
			fprintf(stderr, "Exited %i\n", data[i].retval);

		verbose("Closing library %s... ", data[i].argv[0]);
		if (dlclose(hdl))
			fprintf(stderr, "%s\n", dlerror());
		else
			verbose("done\n");

		data[i].handler = NULL;
	}
}

int main(int argc, char * const argv[])
{
	static struct options options;
	int argi, ret = EXIT_FAILURE;
	static sigset_t sigset;

	argi = parse_arguments(&options, argc, argv);
	if (argi < 0) {
		fprintf(stderr, "Error: %s: Invalid option!\n", argv[optind-1]);
		exit(EXIT_FAILURE);
	} else if (argc - argi < 1) {
		usage(stdout, argv[0]);
		fprintf(stderr, "Error: Too few arguments!\n");
		exit(EXIT_FAILURE);
	}

	if (argc - argi > 0) {
		struct plugin_data data[argc-argi];
		int sig;

		memset(data, 0, sizeof(data));
		PRIVATE.plugins_count = argc-argi;
		PRIVATE.plugins_data = data;
		PRIVATE.main_thread = pthread_self();

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

		if (sigprocmask(SIG_SETMASK, &sigset, NULL) == -1) {
			perror("sigprocmask");
			return EXIT_FAILURE;
		}

		if (start_plugins(data, argc-argi, &argv[argi]))
			goto exit;

		/* main loop */
		/* for (;;); */
		for (;;) {

			siginfo_t siginfo;
			sig = sigwaitinfo(&sigset, &siginfo);
			if (sig == -1) {
				if (errno == EINTR || errno == EAGAIN)
					continue;

				perror("sigwaitinfo");
				break;
			}

			debug("sigwaitinfo(): %i: %s\n", sig, strsignal(sig));

			if ((sig == SIGINT) || (sig == SIGTERM))
				break;

			fprintf(stderr, "%s: Uncaught signal!\n", strsignal(sig));
			break;
		}

		if (sigprocmask(SIG_UNBLOCK, &sigset, NULL) == -1)
			perror("sigprocmask");

		ret = EXIT_SUCCESS;

exit:
		stop_plugins(data, argc-argi, &argv[argi]);
	}

	return ret;
}
