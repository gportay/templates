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

#include "pexec.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#define __close(fd) \
do { \
	if (close(fd) == -1) \
		perror("close"); \
} while (0)

#define __execv(path, argv) \
do { \
	execv(path, argv); \
	perror("execv"); \
	exit(EXIT_FAILURE); \
} while(0)

#define __execvp(path, argv) \
do { \
	execvp(path, argv); \
	perror("execvp"); \
	exit(EXIT_FAILURE); \
} while(0)

#define __execve(path, argv, envp) \
do { \
	execve(path, argv, envp); \
	perror("execve"); \
	exit(EXIT_FAILURE); \
} while(0)

#ifdef _GNU_SOURCE
#define __execvpe(path, argv, envp) \
do { \
	execvpe(path, argv, envp); \
	perror("execvpe"); \
	exit(EXIT_FAILURE); \
} while(0)
#endif

int vpexecl(pid_t *pid, const char *path, int argc, const char *arg0, va_list ap);
int vpexeclp(pid_t *pid, const char *file, int argc, const char *arg0, va_list ap);
int vpexecle(pid_t *pid, const char *path, int argc, const char *arg0, va_list ap);

int vpexecl(pid_t *pid, const char *path, int argc, const char *argv0, va_list ap)
{
	int i;
	char *argv[argc+1];
	va_list ap2;

	/* Construct argv */
	va_copy(ap2, ap);
	argv[0] = (char *)argv0;
	for (i = 1; i < argc; i++)
		argv[i] = va_arg(ap2, char *);
	argv[i] = NULL;
	va_end(ap2);

	return pexecv(pid, path, argv);
}

int vpexeclp(pid_t *pid, const char *file, int argc, const char *argv0, va_list ap)
{
	int i;
	char *argv[argc+1];
	va_list ap2;

	/* Construct argv */
	va_copy(ap2, ap);
	argv[0] = (char *)argv0;
	for (i = 1; i < argc; i++)
		argv[i] = va_arg(ap2, char *);
	argv[i] = NULL;
	va_end(ap2);

	return pexecvp(pid, file, argv);
}

int vpexecle(pid_t *pid, const char *path, int argc, const char *argv0, va_list ap)
{
	int i;
	char *argv[argc+1];
	char **envp;
	va_list ap2;

	/* Construct argv and set envp*/
	va_copy(ap2, ap);
	argv[0] = (char *)argv0;
	for (i = 1; i < argc; i++)
		argv[i] = va_arg(ap2, char *);
	argv[i] = NULL;
	envp = va_arg(ap, char **);
	va_end(ap2);

	return pexecve(pid, path, argv, envp);
}

int pexecl(pid_t *pid, const char *path, const char *argv, ...)
{
	int argc;
	va_list ap;

	/* Count variadic arguments */
	va_start(ap, argv);
	for (argc = 1; va_arg(ap, const char *); argc++);
	va_end(ap);

	return vpexecl(pid, path, argc, argv, ap);
}

int pexeclp(pid_t *pid, const char *file, const char *argv, ...)
{
	int argc;
	va_list ap;

	/* Count variadic argument */
	va_start(ap, argv);
	for (argc = 1; va_arg(ap, const char *); argc++);
	va_end(ap);

	return vpexeclp(pid, file, argc, argv, ap);
}

int pexecle(pid_t *pid, const char *path, const char *argv, ...)
{
	int argc;
	va_list ap;

	/* Count variadic argument */
	va_start(ap, argv);
	for (argc = 1; va_arg(ap, const char *); argc++);
	va_end(ap);

	return vpexecle(pid, path, argc, argv, ap);
}

int pexecv(pid_t *pid, const char *path, char * const argv[])
{
	int fd[2];
	pid_t p;

	if (pipe(fd) == -1) {
		perror("pipe");
		return -1;
	}

	p = fork();
	if (p == -1) {
		perror("fork");
		__close(fd[0]);
		__close(fd[1]);
		return -1;
	}

	/* Parent */
	if (p) {
		if (pid) *pid = p;
		__close(fd[1]);
		return fd[0];
	}

	/* Child */
	__close(STDIN_FILENO);
	__close(STDOUT_FILENO);
	dup2(fd[0], STDIN_FILENO);
	dup2(fd[1], STDOUT_FILENO);
	__execv(path, argv);
}

int pexecvp(pid_t *pid, const char *file, char * const argv[])
{
	int fd[2];
	pid_t p;

	if (pipe(fd) == -1) {
		perror("pipe");
		return -1;
	}

	p = fork();
	if (p == -1) {
		perror("fork");
		__close(fd[0]);
		__close(fd[1]);
		return -1;
	}

	/* Parent */
	if (p) {
		if (pid) *pid = p;
		__close(fd[1]);
		return fd[0];
	}

	/* Child */
	__close(STDIN_FILENO);
	__close(STDOUT_FILENO);
	dup2(fd[0], STDIN_FILENO);
	dup2(fd[1], STDOUT_FILENO);
	__execvp(file, argv);
}

int pexecve(pid_t *pid, const char *path, char *const argv[], char *const envp[])
{
	int fd[2];
	pid_t p;

	if (pipe(fd) == -1) {
		perror("pipe");
		return -1;
	}

	p = fork();
	if (p == -1) {
		perror("fork");
		__close(fd[0]);
		__close(fd[1]);
		return -1;
	}

	/* Parent */
	if (p) {
		if (pid) *pid = p;
		__close(fd[1]);
		return fd[0];
	}

	/* Child */
	__close(STDIN_FILENO);
	__close(STDOUT_FILENO);
	dup2(fd[0], STDIN_FILENO);
	dup2(fd[1], STDOUT_FILENO);
	__execve(path, argv, envp);
}

#ifdef _GNU_SOURCE
int pexecvpe(pid_t *pid, const char *file, char * const argv[], char * const envp[])
{
	int fd[2];
	pid_t p;

	if (pipe(fd) == -1) {
		perror("pipe");
		return -1;
	}

	p = fork();
	if (p == -1) {
		perror("fork");
		__close(fd[0]);
		__close(fd[1]);
		return -1;
	}

	/* Parent */
	if (p) {
		if (pid) *pid = p;
		__close(fd[1]);
		return fd[0];
	}

	/* Child */
	__close(STDIN_FILENO);
	__close(STDOUT_FILENO);
	dup2(fd[0], STDIN_FILENO);
	dup2(fd[1], STDOUT_FILENO);
	__execvpe(file, argv, envp);
}
#endif
