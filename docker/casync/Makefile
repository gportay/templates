#
# Copyright 2019 Gaël PORTAY
#
# SPDX-License-Identifier: LGPL-2.1-or-later
#

SHELL := /bin/bash
RUNOPTS ?= --volume "$(PWD)/www:/var/www/lighttpd"

.PHONY: all
all: httpd

.PHONY: help 
.SILENT: help
help:
	echo  'Cleaning targets:' 
	echo  '  clean           - Remove most generated files but keep the chunck store and'
	echo  '                    casync local build'
	echo  '  mrproper        - Remove all generated files + chunk store'
	echo  '  distclean       - mrproper + remove casync local build'
	echo  ''
	echo  'Generic targets:'
	echo  '  all             - Run the chunk webstore (http://localhost/)'
	echo  '  httpd           - fedora-httpd'
	echo  '  fedora-httpd    - Run fedora copy of nginx'
	echo  '  debian-httpd    - Run debian copy of nginx'
	echo  ''
	echo  'Build targets:'
	echo  '  *.catar         - Create the .catar archive of the /'
	echo  '  *.caidx         - Create the .caidx index and its associated chunk store of'
	echo  '                    the /'
	echo  '  *.caibx         - Create the .caibx index and its associated chunk store of'
	echo  '                    the file'
	echo  'Test targets:'
	echo  '  shell           - Run shell in Docker'
	echo  '  fedora-httpd    - Run shell in fedora docker image'
	echo  '  debian-httpd    - Run shell in debian docker image'
	echo  '  extract_*.catar - Extract the contents of .catar archive to .'
	echo  '  extract_*.caidx - Extract the contents of .catar index to .'
	echo  '  extract_*.caibx - Extract the contents of .caidx index to .extracted'
	echo  '  list_*.catar    - List all files and directories from .catar archive' 
	echo  '  list_*.caidx    - List all files and directories from .caidx index' 

.PHONY: httpd
httpd: fedora-httpd

.PHONY: shell
shell: fedora-shell

fedora-httpd debian-httpd:
%-httpd: override RUNOPTS += --volume "$(PWD)/etc:/etc/lighttpd/" --publish 80:80
%-httpd: %-casync.iid
	docker run --rm $(RUNOPTS) $*

debian-shell fedora-shell:
%-shell: override RUNOPTS += --tty --interactive
%-shell: casync/_build-%/casync | %-casync.iid
	docker run --rm $(RUNOPTS) $*-casync $(SHELL)

.PHONY: host
host: casync/_build-host/casync

.PRECIOUS: casync/_build-host/casync
casync/_build-host/casync: casync/_build-host/build.ninja
	ninja -C $(@D)

.PRECIOUS: casync/_build-host/build.ninja
casync/_build-host/build.ninja: | casync/meson.build
	cd casync && meson _build-host

.PRECIOUS: casync/_build-%/casync
casync/_build-%/casync: casync/_build-%/build.ninja | %-casync.iid
	docker run --rm $(RUNOPTS) --user $$USER $*-casync ninja -C $(@D)

.PRECIOUS: casync/_build-%/build.ninja
casync/_build-%/build.ninja: | casync/meson.build %-casync.iid
	docker run --rm $(RUNOPTS) --user $$USER $*-casync /bin/bash -c 'cd casync && meson _build-$*'

.PRECIOUS: casync/meson.build
casync/meson.build:
	git clone git@github.com:systemd/casync.git

.PHONY: clean
clean:
	rm -f *.iid *.catar *.caidx

.PHONY: distclean
distclean: clean
	rm -Rf default.castr

.PHONY: mrproper
mrproper: distclean
	rm -Rf casync/_build*

list_fedora.catar list_fedora.caidx list_debian.catar list_debian.caidx:
list_%: casync/_build-host/casync
	casync/_build-host/casync list $*

extract_fedora.catar extract_fedora.caidx extract_debian.catar extract_debian.caidx:
extract_%.caidx: casync/_build-%/casync
	mkdir -p root-$*
	docker run --rm $(RUNOPTS) $* /bin/bash -c "cd root-$* && ../casync/_build-$*/casync extract ../$*.caidx"

fedora.catar fedora.caidx debian.catar debian.caidx:
.PRECIOUS: %.catar %.caidx
%.catar %.caidx: %-casync.iid casync/_build-%/casync FORCE
	docker run --rm $(RUNOPTS) $* /bin/bash -c "casync/_build-$*/casync make $@ / && chown -R $$UID:$$UID $@ default.castr/"

extract_%.caibx: casync/_build-host/casync
	../casync/_build-host/casync extract $*.caibx $*.extracted

.PRECIOUS: %.caibx
%.caibx: casync/_build-host/casync FORCE
	casync/_build-host/casync make $@ $*

.PRECIOUS: %-casync.iid
%-casync.iid: Dockerfile-% | casync/meson.build
	docker build --tag $*-casync --iidfile $@ --build-arg USER=$$USER --build-arg UID=$$UID - < $<

.PHONY: FORCE
FORCE:
