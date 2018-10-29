#
#  Copyright (C) 2018 Gaël PORTAY
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

set -e

# Create root CA and and www.portay.io root-signed CA
install -D Makefile build/root/Makefile
cp Makefile build/root
make -C build/root/ CN=root www.portay.io.cert.pem

# Crypt root private key using aes256
openssl rsa -aes256 -in build/root/private/cakey.pem -out build/root/cakey.pem

# Install root CA to output
make -C build/root/ install DESTDIR="$PWD/output/root"

# Install www.portay.io CA context to build/www.portay.io
install -D Makefile build/www.portay.io/Makefile
mkdir -p build/www.portay.io/etc/ssl/private/
cp -a build/root/www.portay.io.key.pem build/www.portay.io/etc/ssl/private/cakey.pem
cp -a build/root/www.portay.io.cert.pem build/www.portay.io/etc/ssl/cacert.pem

# Create www.portay.io client certificates
for email in "$@"
do
	make  -C build/www.portay.io/ "$email.p12"
done

# Install www.portay.io CA to output
make -C build/www.portay.io/ install DESTDIR="$PWD/output/www.portay.io"
