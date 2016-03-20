/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 Gaël PORTAY <gael.portay@gmail.com>
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

#include "Poller.h"

#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "errno_error.h"

#define EVENTS	10

int Poller::__fd = -1;

Poller::Poller()
{
}

Poller::~Poller()
{
	if (Poller::__fd > 0) {
		if (close(Poller::__fd) == -1)
			THROW_ERRNO_ERROR("close");
		Poller::__fd = -1;
	}
}

void Poller::add(Watcher &watcher)
{
	struct epoll_event evt;
	memset(&evt, 0, sizeof(evt));
	evt.events = watcher.__events;
	evt.data.ptr = &watcher;

	if (epoll_ctl(Poller::__fd, EPOLL_CTL_ADD, watcher._fd, &evt) == -1) {
		if (errno != EEXIST)
			THROW_ERRNO_ERROR("epoll_ctl");

		modify(watcher);
	}
}

void Poller::modify(Watcher &watcher)
{
	struct epoll_event evt;
	memset(&evt, 0, sizeof(evt));
	evt.events = watcher.__events;
	evt.data.ptr = &watcher;

	if (epoll_ctl(Poller::__fd, EPOLL_CTL_MOD, watcher._fd, &evt) == -1) {
		if (errno != ENOENT)
			THROW_ERRNO_ERROR("epoll_ctl");

		add(watcher);
	}
}

void Poller::remove(Watcher &watcher)
{
	struct epoll_event evt;
	memset(&evt, 0, sizeof(evt));
	evt.events = 0;
	evt.data.ptr = NULL;

	if (epoll_ctl(Poller::__fd, EPOLL_CTL_DEL, watcher._fd, &evt) == -1) {
		if (errno != ENOENT)
			THROW_ERRNO_ERROR("epoll_ctl");
	}
}

Poller &Poller::get()
{
	static Poller thePoller;

	if (Poller::__fd < 0) {
		Poller::__fd = epoll_create(1);
		if (Poller::__fd < 0)
			THROW_ERRNO_ERROR("epoll_create");
	}

	return thePoller;
}

void Poller::loop(int timeout)
{
	for (;;) {
		struct epoll_event evts[EVENTS];
		int nbfd = epoll_wait(Poller::__fd, evts, EVENTS -1, timeout);
		if (nbfd < 0)
			THROW_ERRNO_ERROR("epoll_wait");

		for (int i = 0; i < nbfd; i++) {
			Watcher *w = static_cast<Watcher *>(evts[i].data.ptr);
			w->onEvent(evts[i].events);
		}
	}
}

