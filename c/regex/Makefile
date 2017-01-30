#
# Copyright 2016 Gaël PORTAY <gael.portay@gmail.com>
#
# Licensed under the MIT license.
#

.PHONY: all
all: mime-types iana-mimes.txt

%.csv:
	wget http://www.iana.org/assignments/media-types/$@

iana-mimes.txt: application.csv audio.csv image.csv message.csv model.csv multipart.csv text.csv video.csv
	cat $^ | cut -d, -f2 | grep '/' >$@

.SILENT: iana-mimes
.PHONY: iana-mimes
iana-mimes: iana-mimes.txt
	echo '/*'
	echo ' * Copyright 2016 Gaël PORTAY <gael.portay@gmail.com>'
	echo ' *'
	echo ' * Licensed under the MIT license.'
	echo ' *'
	echo ' * Automatically generated file; SHOULD NOT EDIT.'
	echo ' */'
	echo ''
	echo '#ifndef __IANA_MIMES_H__'
	echo '#define __IANA_MIMES_H__'
	echo ''
	echo 'const char *IANA_MIMES[] = {'
	sed -e 's/^/\t"/' -e 's/$$/",/' $<
	echo '	NULL'
	echo '};'
	echo ''
	echo '#endif'

iana-mimes.h:
	make --silent iana-mimes >$@

clean:
	rm -f *.csv *.o iana-mimes.txt mime-types.h mime-types

