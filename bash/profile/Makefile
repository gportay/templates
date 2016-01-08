#
# Copyright 2016 Gaël PORTAY <gael.portay@gmail.com>
#
# Licensed under the MIT license.
#

all:
	@echo "Usage: make install" >&2
	@false

install:
	for f in .bashrc .bash_profile; do \
		if [ -f ~/$$f ]; then \
			mv ~/$$f ~/$$f-$(shell date +%Y-%m-%d-%H:%m:%S); \
		fi; \
		install -m 644 $$f ~/; \
	done

