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

static inline
char *argvtonstr(char *dest, char * const src[], size_t n)
{
	char * const *arg = src;
	char *s = dest;

	if (!dest || !src) {
		errno = EINVAL;
		return NULL;
	}

	for (;;) {
		size_t l;
		if (!n || !*arg)
			break;

		s = strncpy(s, *arg, n-1);
		if (!s)
			break;

		l = strlen(*arg) + 1; /* + NUL character*/
		s += l;
		n -= l;
		arg++;
	}

	*s = '\0';
	return dest;
}

static inline
char **strtonargv(char *dest[], char *src, size_t n)
{
	char **arg = dest;
	char *s = src;

	if (!dest || !src) {
		errno = EINVAL;
		return NULL;
	}

	*arg++ = s;
	n--;
	for (;;) {
		if (!n)
			break;

		s = strchr(s, '\0');
		if (!s)
			break;

		s++;
		n--;
		*arg = s;
		arg++;
	}

	*arg = NULL;
	return dest;
}

static inline
ssize_t aswrite(char *dest, char * const src[], size_t n)
{
	char * const *arg = src;
	ssize_t size = 0;
	char *s = dest;

	if (!dest || !src) {
		errno = EINVAL;
		return -1;
	}

	for (;;) {
		size_t l;
		if (!n || !*arg)
			break;

		s = strncpy(s, *arg, n-1);
		if (!s)
			break;

		l = strlen(*arg) + 1; /* + NUL character*/
		s += l;
		n -= l;
		arg++;
		size += l;
	}

	*s = '\0';
	return size;
}

static inline
ssize_t asread(char *dest[], char *src, size_t n)
{
	char **arg = dest;
	ssize_t size = 0;
	char *s = src;

	if (!dest || !src) {
		errno = EINVAL;
		return NULL;
	}

	*arg++ = s;
	n--;
	size++;
	for (;;) {
		if (!n)
			break;

		s = strchr(s, '\0');
		if (!s)
			break;

		s++;
		n--;
		*arg = s;
		arg++;
		size++;
	}

	*arg = NULL;
	return size;
}

/* TODO */
ssize_t  sread(char *str, void *buf, size_t nbyte);
ssize_t swrite(char *str, const void *buf, size_t nbyte);

ssize_t   read(int fildes, void *buf, size_t nbyte);
ssize_t  write(int fildes, const void *buf, size_t nbyte);

ssize_t  pread(int fildes, void *buf, size_t nbyte, off_t offset);
ssize_t pwrite(int fildes, const void *buf, size_t nbyte, off_t offset);

size_t   fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
size_t  fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);

int   printf(const char *format, ...);
int  fprintf(FILE *stream, const char *format, ...);
int  dprintf(int fd, const char *format, ...);
int  sprintf(char *str, const char *format, ...);
int snprintf(char *str, size_t size, const char *format, ...);

int   vprintf(const char *format, va_list ap);
int  vfprintf(FILE *stream, const char *format, va_list ap);
int  vdprintf(int fd, const char *format, va_list ap);
int  vsprintf(char *str, const char *format, va_list ap);
int vsnprintf(char *str, size_t size, const char *format, va_list ap);





void    *memccpy(void *restrict, const void *restrict, int, size_t);
void    *memchr(const void *, int, size_t);
int      memcmp(const void *, const void *, size_t);
void    *memcpy(void *restrict, const void *restrict, size_t);
void    *memmove(void *, const void *, size_t);
void    *memset(void *, int, size_t);
char    *stpcpy(char *restrict, const char *restrict);
char    *stpncpy(char *restrict, const char *restrict, size_t);
char    *strcat(char *restrict, const char *restrict);
char    *strchr(const char *, int);
int      strcmp(const char *, const char *);
int      strcoll(const char *, const char *);
int      strcoll_l(const char *, const char *, locale_t);
char    *strcpy(char *restrict, const char *restrict);
size_t   strcspn(const char *, const char *);
char    *strdup(const char *);
char    *strerror(int);
char    *strerror_l(int, locale_t);
int      strerror_r(int, char *, size_t);
size_t   strlen(const char *);
char    *strncat(char *restrict, const char *restrict, size_t);
int      strncmp(const char *, const char *, size_t);
char    *strncpy(char *restrict, const char *restrict, size_t);
char    *strndup(const char *, size_t);
size_t   strnlen(const char *, size_t);
char    *strpbrk(const char *, const char *);
char    *strrchr(const char *, int);
char    *strsignal(int);
size_t   strspn(const char *, const char *);
char    *strstr(const char *, const char *);
char    *strtok(char *restrict, const char *restrict);
char    *strtok_r(char *restrict, const char *restrict, char **restrict);
size_t   strxfrm(char *restrict, const char *restrict, size_t);
size_t   strxfrm_l(char *restrict, const char *restrict, size_t, locale_t);




/*
 * ex: filetype=c
 */
