/*
 * Copyright (C) 2017  Gaël Portay
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
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

#include "uevent.h"

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <asm/types.h>
#include <linux/netlink.h>

static int event(char *action, char *devpath, void *data)
{
	fprintf((FILE*)data, "%s@%s\n", action, devpath);
	return 0;
}

static int variable(char *variable, char *value, void *data)
{
	fprintf((FILE*)data, "%s=%s\n", variable, value);
	return 0;
}

int main(void)
{
	struct sockaddr_nl addr;
	int fd;

	memset(&addr, 0, sizeof(addr));
	addr.nl_family = AF_NETLINK;
	addr.nl_pid = getpid();
	addr.nl_groups = NETLINK_KOBJECT_UEVENT;

	fd = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_KOBJECT_UEVENT);
	if (fd == -1) {
		perror("socket");
		return 1;
	}

	if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
		perror("bind");
		return 1;
	}

	return uevent_parse(fd, event, variable, stdout);
}
