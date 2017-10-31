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

#ifndef _UEVENT_H_
#define _UEVENT_H_

typedef int uevent_event_cb_t(char *, char *, void *);

typedef int uevent_variable_cb_t(char *, char *, void *);

int uevent_parse(int fd,
	         uevent_event_cb_t *evt_cb,
	         uevent_variable_cb_t *var_cb,
	         void *data);

#endif
