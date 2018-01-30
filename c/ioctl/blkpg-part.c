/*
 *  Copyright (C) 2018 Savoir-Faire Linux Inc.
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
#include <sys/stat.h>
#include <fcntl.h>
#include <stropts.h>
#include <linux/blkpg.h>

struct options_t {
	long long start;
	long long length;
	int pno;
	char *devname;
	char *volname;
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
	fprintf(f, "Usage: %s [OPTIONS] add|resize DEVNAME PNO START LENGTH\n"
		   "       %s [OPTIONS] delete     DEVNAME PNO\n"
		   "\n"
		   "blkpg-part adds, resizes and deletes partitions using the "
		   "following BLKPG_ADD_PARTITION, BLKPG_DEL_PARTITION and "
		   "BLKPG_RESIZE_PARTITION ioctl(3P).\n"
		   "\n"
		   "DEVNAME: partition name, like sda5 or c0d1p2, to be used in kernel messages.\n"
		   "PNO:     partition number.\n"
		   "START:   starting offset in bytes.\n"
		   "LENGTH:  length in bytes.\n"
		   "\n"
		   "Options:\n"
		   " -l or --volume-name LABEL Set volume label.\n"
		   "                           Unused at present (4.15).\n"
		   " -h or --help              Display this message.\n"
		   " -V or --version           Display the version.\n"
		   "", applet(arg0), applet(arg0));
}

int parse_arguments(struct options_t *opts, int argc, char * const argv[])
{
	static const struct option long_options[] = {
		{ "volume-name", required_argument, NULL, 'l' },
		{ "version",     no_argument,       NULL, 'V' },
		{ "help",        no_argument,       NULL, 'h' },
		{ NULL,          no_argument,       NULL, 0   }
	};

	for (;;) {
		int index;
		int c = getopt_long(argc, argv, "l:Vh", long_options, &index);
		if (c == -1) {
			break;
		}

		switch (c) {
		case 'l':
			opts->volname = optarg;
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
			exit(EXIT_FAILURE);
		}
	}

	return optind;
}

int main(int argc, char * const argv[])
{
	static struct options_t options;
	struct blkpg_partition data;
	struct blkpg_ioctl_arg req = {
	        .op = 0,
		.flags = 0,
		.datalen = sizeof(data),
		.data = &data,
	};
	int argi, fd, ret = EXIT_FAILURE;
	char *e;
	long l;

	argi = parse_arguments(&options, argc, argv);
	if (argi < 0) {
		fprintf(stderr, "Error: Invalid argument!\n");
		exit(EXIT_FAILURE);
	} else if (argc - argi < 3) {
		usage(stdout, argv[0]);
		fprintf(stderr, "Error: Too few arguments!\n");
		exit(EXIT_FAILURE);
	} else if (argc - argi > 5) {
		usage(stdout, argv[0]);
		fprintf(stderr, "Error: Too many arguments!\n");
		exit(EXIT_FAILURE);
	}

	if (!strcmp("add", argv[argi])) {
		req.op = BLKPG_ADD_PARTITION;
	} else if (!strcmp("delete", argv[argi])) {
		req.op = BLKPG_DEL_PARTITION;
	} else if (!strcmp("resize", argv[argi])) {
		req.op = BLKPG_RESIZE_PARTITION;
	} else {
		usage(stdout, argv[0]);
		fprintf(stderr, "Error: %s: Invalid operation!\n", argv[argi]);
		exit(EXIT_FAILURE);
	}

	if ((req.op == BLKPG_ADD_PARTITION) ||
	    (req.op == BLKPG_RESIZE_PARTITION)) {
		if (argc - argi < 5) {
			usage(stdout, argv[0]);
			fprintf(stderr, "Error: Too few arguments!\n");
			exit(EXIT_FAILURE);
		}
	}

	argi++;
	options.devname = argv[argi];
	strncpy(data.devname, options.devname, BLKPG_DEVNAMELTH);

	if (options.volname)
		strncpy(data.volname, options.volname, BLKPG_VOLNAMELTH);

	fd = open(data.devname, O_RDWR);
	if (fd == -1) {
		perror("open");
		exit(EXIT_FAILURE);
	}

	argi++;
	l = strtol(argv[argi], &e, 0);
	if (*e) {
		fprintf(stderr, "Error: %s: Invalid argument!\n",
			argv[argi]);
		ret = EXIT_FAILURE;
		goto exit;
	}
	data.pno = options.pno = l;

	if ((req.op == BLKPG_ADD_PARTITION) ||
	    (req.op == BLKPG_RESIZE_PARTITION)) {
		argi++;
		l = strtol(argv[argi], &e, 0);
		if (*e) {
			fprintf(stderr, "Error: %s: Invalid argument!\n",
				argv[argi]);
			ret = EXIT_FAILURE;
			goto exit;
		}
		data.start = options.start = l;

		argi++;
		l = strtol(argv[argi], &e, 0);
		if (*e) {
			fprintf(stderr, "Error: %s: Invalid argument!\n",
				argv[argi]);
			ret = EXIT_FAILURE;
			goto exit;
		}
		data.length = options.length = l;
	}

	printf("devname: %s, pno: %i, start: %lli, length: %lli, volname: %s\n",
	       options.devname, options.pno, options.start, options.length,
	       options.volname);

	if (ioctl(fd, BLKPG, &req)) {
		perror("ioctl");
		ret = EXIT_FAILURE;
		goto exit;
	}

	ret = EXIT_SUCCESS;

exit:
	if (fd != -1)
		if (close(fd) == -1)
			perror("close");

	return ret;
}
