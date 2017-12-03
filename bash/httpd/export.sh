#!/bin/sh
#
#  Copyright (C) 2017 Savoir-Faire Linux Inc.
#
#  Authors:
#      Gaël PORTAY <gael.portay@savoirfairelinux.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 2.1 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

for var in DOCUMENT_ROOT HTTP_COOKIE HTTP_HOST HTTP_REFERER HTTP_USER_AGENT \
           HTTPS QUERY_STRING REMOTE_ADDR REMOTE_HOST REMOTE_PORT REMOTE_USER \
           REQUEST_METHOD REQUEST_URI SCRIPT_FILENAME SCRIPT_NAME SERVER_ADMIN \
           SERVER_NAME SERVER_PORT SERVER_SOFTWARE
do
	echo "$var=$(eval echo \${$var})"
done

IFS=& \
for query in $QUERY_STRING
do
	eval "__$query"
	var="__${query%=*}"
	echo "$var=$(eval echo \${$var})"
done
