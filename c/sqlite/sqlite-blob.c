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
#include <errno.h>
#include <getopt.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <sqlite3.h>

#define __sqlite3_perror(s, db) do { \
	fprintf(stderr, "%s: %s\n", s, sqlite3_errmsg(db)); \
} while (0)

struct options {
	int argc;
	char * const *argv;
	const char *file;
	int extract;
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
	fprintf(f, "Usage: %s [OPTIONS] FILE\n"
		   "\n"
		   "Saves many files together into a single database, and can "
		   "restore individual files\nfrom the database.\n"
		   "\n"
		   "Options:\n"
		   " -f or --file FILE     Set the path to database.\n"
		   " -x or --extract       Extract file from an database.\n"
		   " -h or --help          Display this message.\n"
		   " -V or --version       Display the version.\n"
		   "", applet(arg0));
}

int parse_arguments(struct options *opts, int argc, char * const argv[])
{
	static const struct option long_options[] = {
		{ "file",    required_argument, NULL, 'f' },
		{ "extract", no_argument,       NULL, 'x' },
		{ "version", no_argument,       NULL, 'V' },
		{ "help",    no_argument,       NULL, 'h' },
		{ NULL,      no_argument,       NULL, 0   }
	};

	opterr = 0;
	for (;;) {
		int index;
		int c = getopt_long(argc, argv, "d:xVh", long_options, &index);
		if (c == -1) {
			break;
		}

		switch (c) {
		case 'f':
			opts->file = optarg;
			break;

		case 'x':
			opts->extract = 1;
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

int main(int argc, char * const argv[])
{
	int exist, fd = -1, ret = EXIT_FAILURE;
	static struct options options = {
		.file = "database.sql",
	};
	struct stat buf;
	sqlite3 *db;

	int argi = parse_arguments(&options, argc, argv);
	if (argi < 0) {
		fprintf(stderr, "Error: %s: Invalid option!\n", argv[optind-1]);
		exit(EXIT_FAILURE);
	} else if (argc - argi < 1) {
		usage(stdout, argv[0]);
		fprintf(stderr, "Error: Too few arguments!\n");
		exit(EXIT_FAILURE);
	} else if (argc - argi > 1) {
		usage(stdout, argv[0]);
		fprintf(stderr, "Error: Too many arguments!\n");
		exit(EXIT_FAILURE);
	}

	exist = stat(options.file, &buf) == 0;

	if (sqlite3_open(options.file, &db)) {
		__sqlite3_perror("sqlite3_open_v2", db);
		goto exit;
	}

	if (!exist) {
		const char *sql;

		sql = "CREATE TABLE blobs(file TEXT PRIMARY KEY, content BLOB)";
		if (sqlite3_exec(db, sql, 0, 0, 0)) {
			__sqlite3_perror("sqlite3_exec", db);
			goto exit;
		}
	}

	if (options.extract) {
		for (;;) {
			sqlite3_stmt *stmt;
			const char *sql;

			sql = "SELECT content FROM blobs WHERE file = ?";
			if (sqlite3_prepare(db, sql, -1, &stmt, 0)) {
				__sqlite3_perror("sqlite3_prepare", db);
				goto exit;
			}

			if (sqlite3_bind_text(stmt, 1, argv[optind], -1,
					      SQLITE_STATIC)) {
				__sqlite3_perror("sqlite3_bind_text", db);
				goto exit;
			}

			if (sqlite3_step(stmt) != SQLITE_ROW) {
				__sqlite3_perror("sqlite3_step", db);
				goto exit;
			}

			write(STDOUT_FILENO,
			      sqlite3_column_blob(stmt, 0),
			      sqlite3_column_bytes(stmt, 0));

			if (sqlite3_finalize(stmt) == SQLITE_SCHEMA) {
				__sqlite3_perror("sqlite3_step", db);
				continue;
			}

			break;
		}

		ret = EXIT_SUCCESS;
		goto exit;
	}

	for (;;) {
		unsigned char blob[BUFSIZ];
		sqlite3_stmt *stmt;
		const char *sql;
		ssize_t size;

		fd = open(argv[optind], O_RDONLY);
		if (fd == -1) {
			perror("open");
			goto exit;
		}

		size = read(fd, blob, sizeof(blob));
		if (size == -1) {
			perror("read");
			goto exit;
		}

		sql = "INSERT OR REPLACE INTO blobs(file, content) VALUES(?, ?)";
		if (sqlite3_prepare(db, sql, -1, &stmt, 0) != SQLITE_OK) {
			__sqlite3_perror("sqlite3_prepare", db);
			goto exit;
		}

		if (sqlite3_bind_text(stmt, 1, argv[optind], -1,
				      SQLITE_STATIC)) {
			__sqlite3_perror("sqlite3_bind_text", db);
			goto exit;
		}

		if (sqlite3_bind_blob(stmt, 2, blob, size, SQLITE_STATIC)) {
			__sqlite3_perror("sqlite3_bind_blob", db);
			goto exit;
		}

		if (sqlite3_step(stmt) != SQLITE_DONE) {
			__sqlite3_perror("sqlite3_step", db);
			goto exit;
		}

		if (sqlite3_finalize(stmt) == SQLITE_SCHEMA) {
			__sqlite3_perror("sqlite3_step", db);
			continue;
		}

		break;
	}

	ret = EXIT_SUCCESS;

exit:
	if (fd != -1) {
		if (close(fd))
			perror("close");
		fd = -1;
	}

	sqlite3_close(db);
	return ret;
}
