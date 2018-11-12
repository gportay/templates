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
#include <fcntl.h>
#include <dirent.h>

#include <linux/limits.h>

#include <menu.h>

#include "nconf.h"

#define __strcpy(d, s) strncpy(d, s, sizeof(d))

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
	fprintf(f, "Usage: %s [OPTIONS]\n"
		   "\n"
		   "Options:\n"
		   " -t or --timeout SEC   Set timeout in sec [default=5].\n"
		   " -h or --help          Display this message.\n"
		   " -V or --version       Display the version.\n"
		   "\n"
		   "Signals:\n"
		   "SIGTERM                Terminate.\n"
		   "", applet(arg0));
}

int parse_arguments(struct options *opts, int argc, char * const argv[])
{
	static const struct option long_options[] = {
		{ "timeout", required_argument, NULL, 't' },
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
		int c = getopt_long(argc, argv, "t:Vh", long_options, &index);
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

	opts->argc = argc - optind;
	opts->argv = argv;
	return optind;
}

int f_ls(FILE *f, const char *path, int (*filter)(const struct dirent *),
	int (*compar)(const struct dirent **, const struct dirent **))
{

	struct dirent **namelist;
	int i, n;

	n = scandir(path, &namelist, filter, compar);
	if (n == -1) {
		perror("scandir");
		return -1;
	}

	for (i = 0; i < n; i++) {
		fprintf(f, "%s\n", namelist[i]->d_name);
		free(namelist[i]);
	}
	free(namelist);

	return 0;
}

int ls(const char *path)
{
	return f_ls(stdout, path, NULL, alphasort);
}

#include <sys/stat.h>
#include <sys/sysmacros.h>
int mvwprintw_stat(WINDOW *w, int x, int y, const char *path)
{
	struct stat buf;

	if (lstat(path, &buf)) {
		perror("lstat");
		return -1;
	}

	return mvwprintw(w, x, y,
	       "%s %llu %llu %lx %lu %lu %llx %llu %lu %lx %lx %lu %lu %lu %lu",
	       path,
	       buf.st_size,
	       buf.st_blocks,
	       buf.st_mode,
	       buf.st_uid,
	       buf.st_gid,
	       buf.st_dev,
	       buf.st_ino,
	       buf.st_nlink,
	       major(buf.st_rdev),
	       minor(buf.st_rdev),
	       buf.st_atime,
	       buf.st_mtime,
	       buf.st_ctime,
	       buf.st_blksize);
}

int mvprintw_stat(int x, int y, const char *path)
{
	return mvwprintw(stdscr, x, y, path);
}

#include <pwd.h>
#include <grp.h>
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

int main(int argc, char * const argv[])
{
	int flags, sig, argi, ret = EXIT_FAILURE;
	static struct options options;
	int h = 0, min = 0, sec = 0;
	static sigset_t sigset;
	char cwd[PATH_MAX];

	argi = parse_arguments(&options, argc, argv);
	if (argi < 0) {
		fprintf(stderr, "Error: %s: Invalid argument!\n",
			argv[optind-1]);
		exit(EXIT_FAILURE);
	} else if (argc - argi > 1) {
		usage(stdout, argv[0]);
		fprintf(stderr, "Error: Too many arguments!\n");
		exit(EXIT_FAILURE);
	}

	if (argc - argi == 1) {
		if (chdir(options.argv[1])) {
			perror("chdir");
			exit(EXIT_FAILURE);
		}
	}

	if (!getcwd(cwd, sizeof(cwd))) {
		perror("getcwd");
		exit(EXIT_FAILURE);
	}

	if (sigemptyset(&sigset) == -1) {
		perror("sigemptyset");
		exit(EXIT_FAILURE);
	}

	sig = SIGTERM;
	if (sigaddset(&sigset, sig) == -1) {
		perror("sigaddset");
		exit(EXIT_FAILURE);
	}

	sig = SIGINT;
	if (sigaddset(&sigset, sig) == -1) {
		perror("sigaddset");
		exit(EXIT_FAILURE);
	}

	sig = SIGQUIT;
	if (sigaddset(&sigset, sig) == -1) {
		perror("sigaddset");
		exit(EXIT_FAILURE);
	}

	sig = SIGUSR1;
	if (sigaddset(&sigset, sig) == -1) {
		perror("sigaddset");
		exit(EXIT_FAILURE);
	}

	sig = SIGUSR2;
	if (sigaddset(&sigset, sig) == -1) {
		perror("sigaddset");
		exit(EXIT_FAILURE);
	}

	sig = SIGIO;
	if (sigaddset(&sigset, sig) == -1) {
		perror("sigaddset");
		exit(EXIT_FAILURE);
	}

	sig = SIGALRM;
	if (sigaddset(&sigset, sig) == -1) {
		perror("sigaddset");
		exit(EXIT_FAILURE);
	}

	sig = SIGWINCH;
	if (sigaddset(&sigset, sig) == -1) {
		perror("sigaddset");
		exit(EXIT_FAILURE);
	}

	if (sigprocmask(SIG_SETMASK, &sigset, NULL) == -1) {
		perror("sigprocmask");
		exit(EXIT_FAILURE);
	}

	if (fcntl(STDIN_FILENO, F_SETSIG, SIGIO) == -1) {
		perror("fcntl");
		exit(EXIT_FAILURE);
	}

	if (fcntl(STDIN_FILENO, F_SETOWN, getpid()) == -1) {
		perror("fcntl");
		exit(EXIT_FAILURE);
	}

	flags = fcntl(STDIN_FILENO, F_GETFL);
	if (flags == -1) {
		perror("fcntl");
		exit(EXIT_FAILURE);
	}

	flags |= (O_ASYNC | O_NONBLOCK | O_CLOEXEC);
	if (fcntl(STDIN_FILENO, F_SETFL, flags) == -1) {
		perror("fcntl");
		exit(EXIT_FAILURE);
	}

	initscr();
	if (has_colors() == FALSE) {
		endwin();
		fprintf(stderr, "Error: %s does not support color!\n",
			ttyname(STDOUT_FILENO));
		exit(EXIT_FAILURE);
	}
	start_color();
//	cbreak();
	raw();
	noecho();
	clear();
	curs_set(0); /* invisible cursor */
	keypad(stdscr, TRUE); /* function keys */
//	keypad(stdscr, FALSE); /* function keys */
	mousemask(ALL_MOUSE_EVENTS, NULL); /* visible mouse events */
#if NCURSES_REENTRANT
	set_escdelay(1);
#else
	ESCDELAY = 1;
#endif

//	set_colors();

	struct dirent **namelist;
	ITEM **items;
	MENU *menu;
	int i, n;

	n = scandir(cwd, &namelist, NULL, alphasort);
	if (n == -1) {
		def_prog_mode();
		endwin();
		perror("scandir");
		exit(EXIT_FAILURE);
	}

	items = malloc((n+1) * sizeof(ITEM*));
	if (!items) {
		def_prog_mode();
		endwin();
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	i = n;
	items[i] = NULL;
	while (i--) {
		items[i] = new_item(namelist[i]->d_name, namelist[i]->d_name);
		if (!items[i]) {
			def_prog_mode();
			endwin();
			perror("new_item");
			exit(EXIT_FAILURE);
		}

		if (set_item_userptr(items[i], namelist[i])) {
			def_prog_mode();
			endwin();
			perror("set_item_userptr");
			exit(EXIT_FAILURE);
		}
	}

	menu = new_menu((ITEM **)items);
	if (!menu) {
		def_prog_mode();
		endwin();
		perror("new_menu");
		exit(EXIT_FAILURE);
	}

	menu_opts_off(menu, O_SHOWDESC);
	menu_opts_on(menu, O_SHOWMATCH);
	menu_opts_on(menu, O_ONEVALUE);
	menu_opts_on(menu, O_NONCYCLIC);
	menu_opts_on(menu, O_IGNORECASE);
	set_menu_mark(menu, "");
#if 0
	set_menu_fore(menu, attributes[MAIN_MENU_FORE]);
	set_menu_back(menu, attributes[MAIN_MENU_BACK]);
	set_menu_grey(menu, attributes[MAIN_MENU_GREY]);
#endif

	for (;;) {
		siginfo_t siginfo;

		alarm(1);
//		clrtoeol();

		/* Post the menu */
		mvprintw(LINES - 3, 0, "Press <ENTER> to see the option selected");
		mvprintw(LINES - 2, 0, "Up and Down arrow keys to navigate (<ESC> to Exit)");
		mvprintw(LINES - 1, COLS - 8, "%02i%c%02i%c%02i\n", h, sec % 2 ? ':' : ' ', min, sec % 2 ? ':' : ' ', sec);
		post_menu(menu);

		refresh();
		sig = sigtimedwait(&sigset, &siginfo, &options.timeout);
		if (sig == -1) {
			if (errno == EINTR || errno == EAGAIN)
				continue;

			def_prog_mode();
			endwin();
			perror("sigtimedwait");
			break;
		}

		if ((sig == SIGTERM) || (sig == SIGINT) || (sig == SIGQUIT)) {
//		if ((sig == SIGTERM) || (sig == SIGINT)) {
//		if (sig == SIGTERM) {
			def_prog_mode();
			endwin();
			fprintf(stderr, "%s\n", strsignal(sig));
			ret = EXIT_SUCCESS;
			break;
		}

		if (sig == SIGWINCH) {
			def_prog_mode();
			endwin();
			fprintf(stderr, "%s\n", strsignal(sig));
			ret = EXIT_SUCCESS;
			break;
		}

		if (sig == SIGUSR1) {
			if (options.timeout.tv_sec < (time_t)-1)
				options.timeout.tv_sec++;
			continue;
		}

		if (sig == SIGUSR2) {
			if (options.timeout.tv_sec > 0)
				options.timeout.tv_sec--;
			continue;
		}

		if (sig == SIGALRM) {
			sec++;
			if (sec == 60) {
				sec = 0;
				min++;
				if (min == 60) {
					min = 0;
					h++;
				}
			}

			continue;
		}

		if (sig == SIGIO) {
			int c = getch();
			if (c == ERR) {
				def_prog_mode();
				endwin();
				perror("getch");
			}

			if (c == KEY_MOUSE) {
				MEVENT evt;

				if(getmouse(&evt) == OK)
//					if(evt.bstate & BUTTON1_PRESSED)
						printw("LINES: %i, COLS: %i\n", LINES, COLS);

				refresh();
				continue;
			}

			mvprintw(LINES - 5, 0, "c: %c (%i, 0x%x)\n", c, c, c);
			refresh();
			switch (c) {
			/* INTR */
			case 0x03:
				if (kill(getpid(), SIGINT)) {
					def_prog_mode();
					endwin();
					perror("kill");
					exit(EXIT_FAILURE);
				}
				break;
			/* ESC */
			case 0x1b:
				if (kill(getpid(), SIGTERM)) {
					def_prog_mode();
					endwin();
					perror("kill");
					exit(EXIT_FAILURE);
				}
				break;
			/* QUIT */
			case 0x1c:
				if (kill(getpid(), SIGQUIT)) {
					def_prog_mode();
					endwin();
					perror("kill");
					exit(EXIT_FAILURE);
				}
				break;
			case KEY_RESIZE:
				if (kill(getpid(), SIGWINCH)) {
					def_prog_mode();
					endwin();
					perror("kill");
					exit(EXIT_FAILURE);
				}
				break;
			case 0x0a:
			case KEY_ENTER: {
				struct dirent *p;
				ITEM *cur;

				cur = current_item(menu);
				p = item_userptr(cur);
//				p((char *)item_name(cur));
				pos_menu_cursor(menu);
mvprintw(LINES - 5, 0, "KEY_ENTER: %i, c: '\\n' (%i, 0%o, 0x%x), item_name: %s ", KEY_ENTER, c, c, c, (char *)item_name(cur));

				switch (p->d_type) {
				case DT_BLK:
					mvprintw(LINES - 5, 0, "This is a block device.");
					break;
				case DT_CHR:
					mvprintw(LINES - 5, 0, "This is a character device.");
					break;
				case DT_DIR:
					mvprintw(LINES - 5, 0, "This is a directory.");
					break;
				case DT_FIFO:
					mvprintw(LINES - 5, 0, "This is a named pipe (FIFO).");
					break;
				case DT_LNK:
					mvprintw(LINES - 5, 0, "This is a symbolic link.");
					break;
				case DT_REG:
					mvprintw(LINES - 5, 0, "This is a regular file.");
					break;
				case DT_SOCK:
					mvprintw(LINES - 5, 0, "This is a UNIX domain socket.");
					break;
				case DT_UNKNOWN:
					mvprintw(LINES - 5, 0, "The file type could not be determined.");
					break;
				}
				refresh();
				break;
			}

#if 0
			case KEY_LEFT:
				break;
			case KEY_RIGHT:
				break;
#endif
			case KEY_UP: {
				ITEM *cur;

				menu_driver(menu, REQ_UP_ITEM);
				cur = current_item(menu);
				mvwprintw_stat(stdscr, LINES - 6, 0,
					      (char *)item_name(cur));
				refresh();
				break;
			}

			case KEY_DOWN: {
				ITEM *cur;

				menu_driver(menu, REQ_DOWN_ITEM);
				cur = current_item(menu);
				mvwprintw_stat(stdscr, LINES - 6, 0,
					      (char *)item_name(cur));
				refresh();
				break;
			}

			default:
mvprintw(LINES - 5, 0, "default: c: '%c' (%i, 0%o, 0x%x)\n", c == '\n' ? 'n' : c, c, c, c);
				break;
			}

			post_menu(menu);
			continue;
		}

		/* Uncaught signal */
		def_prog_mode();
		endwin();
		fprintf(stderr, "%s: Uncaught signal!\n", strsignal(sig));
		break;
	}

	unpost_menu(menu);

free_menu:
	if (free_menu(menu))
		perror("free_menu");

free_item:
	i = n;
	while (i--) {
		if (free_item(items[i]))
			perror("free_item");
	}
	free(items);

free_dirent:
	i = n;
	while (i--)
		free(namelist[i]);
	free(namelist);

mask:
	if (sigprocmask(SIG_UNBLOCK, &sigset, NULL) == -1)
		perror("sigprocmask");

	return ret;
}
