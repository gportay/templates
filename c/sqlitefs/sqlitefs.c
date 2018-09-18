/*
 *  Copyright (C) 2018 Savoir-Faire Linux Inc.
 *
 *  Authors:
 *       Gaël PORTAY <gael.portay@savoirfairelinux.com>
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

#define FUSE_USE_VERSION 30

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
#include <pwd.h>
#include <grp.h>

#include <fuse.h>
#include <sqlite3.h>

static const char *mode(mode_t mode)
{
	if (S_ISREG(mode))
		return "regular file";

	if (S_ISDIR(mode))
		return "directory";

	if (S_ISBLK(mode))
		return "block special file";

	if (S_ISCHR(mode))
		return "character special file";

	if (S_ISFIFO(mode))
		return "fifo";

	if (S_ISLNK(mode))
		return "symbolic link";

	if (S_ISSOCK(mode))
		return "socket";

	return "unknown";
}

static const char *uid_r(const uid_t uid, char *buf, size_t bufsize)
{
	const char *name = "unknown";
	char pwdbuf[BUFSIZ];
	struct passwd *pwds;
	struct passwd pwd;

	if (getpwuid_r(uid, &pwd, pwdbuf, sizeof(pwdbuf), &pwds) == 0)
		name = pwd.pw_name;

	snprintf(buf, bufsize, "%5i/%8s", uid, name);

	return buf;
}

static const char *gid_r(const gid_t gid, char *buf, size_t bufsize)
{
	const char *name = "unknown";
	char grpbuf[BUFSIZ];
	struct group *grps;
	struct group grp;

	if (getgrgid_r(gid, &grp, grpbuf, sizeof(grpbuf), &grps) == 0)
		name = grp.gr_name;

	snprintf(buf, bufsize, "%5i/%8s", gid, name);

	return buf;
}

static const char *timespec_r(const struct timespec *ts, char *buf,
			      size_t bufsize)
{
	struct tm tm;
	size_t n;

	if (!localtime_r(&ts->tv_sec, &tm))
		return NULL;

	n = strftime(buf, bufsize, "%Y-%m-%d %H:%M:%S", &tm);
	if (!n)
		return NULL;

	if (n < bufsize)
		snprintf(buf+n, bufsize-n, ".%09li", ts->tv_nsec);

	return buf;
}

static int fprintstat(FILE *f, const char *path, const struct stat *buf)
{
	char atimbuf[BUFSIZ];
	char mtimbuf[BUFSIZ];
	char ctimbuf[BUFSIZ];
	char gidbuf[BUFSIZ];
	char uidbuf[BUFSIZ];

	return fprintf(f, "  File: %s\n"
			  "  Size: %li\tBlocks: %li\tIO Block: %li\t%s\n"
			  "Device: %lx/%li\tInode: %lu\tLinks: %li\n"
			  "Access: (O%04o)\tUid: (%s)\tGid: (%s)\n"
			  "Access: %s\n"
			  "Modify: %s\n"
			  "Change: %s\n"
			  " Birth: -\n",
			  path, buf->st_size, buf->st_blocks, buf->st_blksize, mode(buf->st_mode),
			  buf->st_rdev, buf->st_rdev, buf->st_ino, buf->st_nlink,
			  buf->st_mode, uid_r(buf->st_uid, uidbuf, sizeof(uidbuf)), gid_r(buf->st_gid, gidbuf, sizeof(gidbuf)),
			  timespec_r(&buf->st_atim, atimbuf, sizeof(atimbuf)),
			  timespec_r(&buf->st_mtim, mtimbuf, sizeof(mtimbuf)),
			  timespec_r(&buf->st_ctim, ctimbuf, sizeof(ctimbuf)));
}

struct getattr_data {
	int error;
	struct stat *st;
};

static int getattr_cb(void *data, int argc, char **argv, char **colname)
{
	struct getattr_data *pdata = (struct getattr_data *)data;
	int i;
	(void)argc;
	(void)argv;
	(void)colname;

	i = 1;
	pdata->error = 0;
	pdata->st->st_dev = strtol(argv[i++], NULL, 0);
	pdata->st->st_ino = strtol(argv[i++], NULL, 0);
	pdata->st->st_mode = strtol(argv[i++], NULL, 0);
	pdata->st->st_nlink = strtol(argv[i++], NULL, 0);
	pdata->st->st_uid = strtol(argv[i++], NULL, 0);
	pdata->st->st_gid = strtol(argv[i++], NULL, 0);
	pdata->st->st_rdev = strtol(argv[i++], NULL, 0);
	pdata->st->st_size = strtol(argv[i++], NULL, 0);
	pdata->st->st_blksize = strtol(argv[i++], NULL, 0);
	pdata->st->st_blocks = strtol(argv[i++], NULL, 0);
	pdata->st->st_atim.tv_sec = strtol(argv[i++], NULL, 0);
	pdata->st->st_atim.tv_nsec = strtol(argv[i++], NULL, 0);
	pdata->st->st_mtim.tv_sec = strtol(argv[i++], NULL, 0);
	pdata->st->st_mtim.tv_nsec = strtol(argv[i++], NULL, 0);
	pdata->st->st_ctim.tv_sec = strtol(argv[i++], NULL, 0);
	pdata->st->st_ctim.tv_nsec = strtol(argv[i++], NULL, 0);

	return SQLITE_OK;
}

struct readdir_data {
	const char *parent;
	void *buffer;
	fuse_fill_dir_t filler;
};

static int readdir_cb(void *data, int argc, char **argv, char **colname)
{
	struct readdir_data *pdata = (struct readdir_data *)data;
	size_t len;
	int i;
	(void)colname;

	len = strlen(pdata->parent);
	for (i = 0; i < argc; i++) {
		if (strcmp(pdata->parent, argv[0]) == 0)
			continue;

		pdata->filler(pdata->buffer, &argv[0][len], NULL, 0);
	}

	return SQLITE_OK;
}

static int add_file(sqlite3 *db, const char *file, const char *parent,
		    const struct stat *st)
{
	char sql[BUFSIZ];
	char *e;

	snprintf(sql, sizeof(sql), "INSERT OR REPLACE INTO files(path, parent, "
		 "st_dev, st_ino, st_mode, st_nlink, st_uid, st_gid, st_rdev, "
		 "st_size, st_blksize, st_blocks, st_atim_sec, st_atim_nsec, "
		 "st_mtim_sec, st_mtim_nsec, st_ctim_sec, st_ctim_nsec) "
		 "VALUES(\"%s\", \"%s\", %lu, %lu, %u, %lu, %u, %u, %lu, %lu, "
		 "%lu, %lu, %lu, %lu, %lu, %lu, %lu, %lu);",
		 file, parent, st->st_dev, st->st_ino, st->st_mode,
		 st->st_nlink, st->st_uid, st->st_gid, st->st_rdev, st->st_size,
		 st->st_blksize, st->st_blocks, st->st_atim.tv_sec,
		 st->st_atim.tv_nsec, st->st_mtim.tv_sec, st->st_mtim.tv_nsec,
		 st->st_ctim.tv_sec, st->st_ctim.tv_nsec);
	if (sqlite3_exec(db, sql, NULL, 0, &e) != SQLITE_OK) {
		fprintf(stderr, "sqlite3_exec: %s\n", e);
		sqlite3_free(e);
		return 1;
	}

	return 0;
}

/** Get file attributes.
 *
 * Similar to stat().  The 'st_dev' and 'st_blksize' fields are
 * ignored.	 The 'st_ino' field is ignored except if the 'use_ino'
 * mount option is given.
 */
static int sqlitefs_getattr(const char *path, struct stat *st)
{
	sqlite3 *db = fuse_get_context()->private_data;
	struct getattr_data data = {
		errno = ENOENT,
		st = st,
	};
	char sql[BUFSIZ];
	char *e;
	int ret;

	if (!db) {
		fprintf(stderr, "%s: Invalid context\n", __FUNCTION__);
		errno = EINVAL;
		return -1;
	}

	snprintf(sql, sizeof(sql), "SELECT "
					"path, "
					"st_dev, "
					"st_ino, "
					"st_mode, "
					"st_nlink, "
					"st_uid, "
					"st_gid, "
					"st_rdev, "
					"st_size, "
					"st_blksize, "
					"st_blocks, "
					"st_atim_sec, "
					"st_atim_nsec, "
					"st_mtim_sec, "
					"st_mtim_nsec, "
					"st_ctim_sec, "
					"st_ctim_nsec "
				   "FROM files WHERE path = \"%s\";", path);
	ret = sqlite3_exec(db, sql, getattr_cb, &data, &e);
	if (ret != SQLITE_OK) {
		fprintf(stderr, "sqlite3_exec: %s\n", e);
		sqlite3_free(e);
		errno = EACCES;
		return -1;
	}

	if (data.error) {
		errno = data.error;
		return -1;
	}

	fprintstat(stderr, path, st);

	return 0;
}

/** Read directory
 *
 * This supersedes the old getdir() interface.  New applications
 * should use this.
 *
 * The filesystem may choose between two modes of operation:
 *
 * 1) The readdir implementation ignores the offset parameter, and
 * passes zero to the filler function's offset.  The filler
 * function will not return '1' (unless an error happens), so the
 * whole directory is read in a single readdir operation.  This
 * works just like the old getdir() method.
 *
 * 2) The readdir implementation keeps track of the offsets of the
 * directory entries.  It uses the offset parameter and always
 * passes non-zero offset to the filler function.  When the buffer
 * is full (or an error happens) the filler function will return
 * '1'.
 *
 * Introduced in version 2.3
 */
static int sqlitefs_readdir(const char *path, void *buffer,
			    fuse_fill_dir_t filler, off_t offset,
			    struct fuse_file_info *fi)
{
	sqlite3 *db = fuse_get_context()->private_data;
	struct readdir_data data = {
		.parent = path,
		.buffer = buffer,
		.filler = filler,
	};
	char sql[BUFSIZ];
	char *e;
	int ret;
	(void)offset;
	(void)fi;

	if (!db) {
		fprintf(stderr, "%s: Invalid context\n", __FUNCTION__);
		errno = EINVAL;
		return -1;
	}

	filler(buffer, ".", NULL, 0);
	filler(buffer, "..", NULL, 0);

	snprintf(sql, sizeof(sql),
		 "SELECT path FROM files WHERE parent = \"%s\";", path);
	ret = sqlite3_exec(db, sql, readdir_cb, &data, &e);
	if (ret != SQLITE_OK) {
		fprintf(stderr, "sqlite3_exec: %s\n", e);
		sqlite3_free(e);
		errno = EACCES;
		return -1;
	}

	return 0;
}

/**
 * Initialize filesystem
 *
 * The return value will passed in the private_data field of
 * fuse_context to all file operations and as a parameter to the
 * destroy() method.
 *
 * Introduced in version 2.3
 * Changed in version 2.6
 */
static void *sqlitefs_init(struct fuse_conn_info *conn)
{
	char sql[BUFSIZ];
	struct stat st;
	sqlite3 *db;
	int exists;
	char *e;
	(void)conn;

	exists = stat("fs.db", &st) == 0;

	if (sqlite3_open("fs.db", &db)) {
		fprintf(stderr, "sqlite3_open: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return NULL;
	}

	if (!exists) {
		snprintf(sql, sizeof(sql),
		       "CREATE TABLE IF NOT EXISTS files("
				"path TEXT NOT NULL PRIMARY KEY, "
				"parent TEXT NOT NULL, "
				"st_dev INT(8), "
				"st_ino INT(8), "
				"st_mode INT(4), "
				"st_nlink INT(8), "
				"st_uid INT(4), "
				"st_gid INT(4), "
				"st_rdev INT(8), "
				"st_size INT(8), "
				"st_blksize INT(8), "
				"st_blocks INT(8), "
				"st_atim_sec INT(8), "
				"st_atim_nsec INT(8), "
				"st_mtim_sec INT(8), "
				"st_mtim_nsec INT(8), "
				"st_ctim_sec INT(8), "
				"st_ctim_nsec INT(8));");
		if (sqlite3_exec(db, sql, NULL, 0, &e) != SQLITE_OK) {
			fprintf(stderr, "sqlite3_exec: %s\n", e);
			sqlite3_free(e);
			sqlite3_close(db);
			return NULL;
		}

		memset(&st, 0, sizeof(struct stat));
		/* Ignored st.st_dev = 0; */
		/* Ignored st.st_ino = 0; */
		st.st_mode = S_IFDIR | 0755;
		st.st_nlink = 2;
		st.st_uid = getuid();
		st.st_gid = getgid();
		/* Ignored st.st_blksize = 0; */
		st.st_atime = time(NULL);
		st.st_mtime = time(NULL);
		st.st_ctime = time(NULL);

		snprintf(sql, sizeof(sql),
			"INSERT OR REPLACE INTO files("
				"path, "
				"parent, "
				"st_dev, "
				"st_ino, "
				"st_mode, "
				"st_nlink, "
				"st_uid, "
				"st_gid, "
				"st_rdev, "
				"st_size, "
				"st_blksize, "
				"st_blocks, "
				"st_atim_sec, "
				"st_atim_nsec, "
				"st_mtim_sec, "
				"st_mtim_nsec, "
				"st_ctim_sec, "
				"st_ctim_nsec)"
			"VALUES(\"%s\", \"%s\", %lu, %lu, %u, %lu, %u, %u, %lu, %lu, %lu, %lu, %lu, %lu, %lu, %lu, %lu, %lu);",
				"/",
				"/",
				st.st_dev,
				st.st_ino,
				st.st_mode,
				st.st_nlink,
				st.st_uid,
				st.st_gid,
				st.st_rdev,
				st.st_size,
				st.st_blksize,
				st.st_blocks,
				st.st_atim.tv_sec,
				st.st_atim.tv_nsec,
				st.st_mtim.tv_sec,
				st.st_mtim.tv_nsec,
				st.st_ctim.tv_sec,
				st.st_ctim.tv_nsec
			);
		if (sqlite3_exec(db, sql, NULL, 0, &e) != SQLITE_OK) {
			fprintf(stderr, "sqlite3_exec: %s\n", e);
			sqlite3_free(e);
			sqlite3_close(db);
			return NULL;
		}

		if (add_file(db, "/.Trash", "/", &st)) {
			sqlite3_close(db);
			return NULL;
		}
	}

	return db;
}

/**
 * Clean up filesystem
 *
 * Called on filesystem exit.
 *
 * Introduced in version 2.3
 */
static void sqlitefs_destroy(void *ptr)
{
	sqlite3 *db = (sqlite3 *)ptr;

	if (!db) {
		fprintf(stderr, "%s: Invalid context\n", __FUNCTION__);
		return;
	}

	sqlite3_close(db);
}

static struct fuse_operations operations = {
	.getattr = sqlitefs_getattr,
	.readdir = sqlitefs_readdir,
	.init = sqlitefs_init,
	.destroy = sqlitefs_destroy,
};

int main(int argc, char *argv[])
{
	return fuse_main(argc, argv, &operations, NULL);
}
