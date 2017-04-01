#
# Copyright 2017 Gaël PORTAY <gael.portay@savoirfairelinux.com>
#
# Licensed under the MIT license.
#

.PHONY: all
all: ssl

.PHONY: nossl
nossl:
	docker run --rm --tty --interactive \
		--publish 80:80 \
		--volume $$PWD/htdocs:/var/www/localhost/htdocs \
		sebp/lighttpd

.PHONY: ssl
ssl: .build root/client.p12 | root data
	docker run --rm --tty --interactive \
		--publish 443:443 \
		--volume $$PWD/root:/root \
		--volume $$PWD/lighttpd:/etc/lighttpd \
		--volume $$PWD/data:/mnt \
		--volume $$PWD/htdocs:/var/www/localhost/htdocs \
		lighttpd

root/client.p12: .build | root data
	docker run --rm --tty --interactive \
		--publish 443:443 \
		--workdir /root/ \
		--volume $$PWD/root:/root \
		--volume $$PWD/lighttpd:/etc/lighttpd \
		--volume $$PWD/data:/mnt \
		--volume $$PWD/htdocs:/var/www/localhost/htdocs \
		lighttpd genkeys.sh

.PHONY: sh
sh: .build | root data
	docker run --rm --tty --interactive \
		--publish 443:443 \
		--workdir /root/ \
		--volume $$PWD/root:/root \
		--volume $$PWD/lighttpd:/etc/lighttpd \
		--volume $$PWD/data:/mnt \
		--volume $$PWD/htdocs:/var/www/localhost/htdocs \
		lighttpd /bin/sh

.build: Dockerfile
	docker build --quiet --tag lighttpd .
	touch $@

root data:
	mkdir -p $@

.PHONY: clean
clean:
	rm -f .build

.PHONY: mrproper
mrproper: clean
	-docker rmi --force lighttpd
	rm -rf root/ lighttpd/server.pem lighttpd/ssl/
