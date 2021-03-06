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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <dlfcn.h>

#include "plugin.h"
#include "strtonargv.h"

static int VERBOSE = 0;
static int DEBUG = 0;
#define verbose(fmt, ...) if (VERBOSE) fprintf(stderr, fmt, ##__VA_ARGS__)
#define debug(fmt, ...) if (DEBUG) fprintf(stderr, fmt, ##__VA_ARGS__)

struct plugin_data {
	char buffer[BUFSIZ];
	int argc;
	char *argv[127];
	void *handler;
	struct plugin *plugin;
};

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
		sym->entrypoint(data[i].argc, data[i].argv);
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
		memset(data, 0, sizeof(data));

		if (start_plugins(data, argc-argi, &argv[argi]))
			goto exit;

		/* main loop */
		/* for (;;); */

		ret = EXIT_SUCCESS;

exit:
		stop_plugins(data, argc-argi, &argv[argi]);
	}

	return ret;
}
