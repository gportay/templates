#
# Copyright 2016 Gaël PORTAY <gael.portay@gmail.com>
#
# Licensed under the MIT license.
#

CFLAGS	+= -Werror -Wall -Wextra

.SILENT: run-test

all: version-lt

version-lt:

run-test: version-lt
	  ./$<         0.9   1.0   $(EXTRAOPTS)
	  ./$< --equal 0.9   1.0   $(EXTRAOPTS)
	  ./$<         0.9   1.0.0 $(EXTRAOPTS)
	  ./$< --equal 0.9   1.0.0 $(EXTRAOPTS)
	  ./$<         0.9   1.0.0 $(EXTRAOPTS)
	  ./$< --equal 0.9   1.0.0 $(EXTRAOPTS)
	  ./$<         0.9.0 1.0   $(EXTRAOPTS)
	  ./$< --equal 0.9.0 1.0   $(EXTRAOPTS)
	! ./$<         1.0   1.0   $(EXTRAOPTS)
	  ./$< --equal 1.0   1.0   $(EXTRAOPTS)
	  ./$<         1.0   1.0.0 $(EXTRAOPTS)
	  ./$< --equal 1.0   1.0.0 $(EXTRAOPTS)
	! ./$<         1.0.0 1.0   $(EXTRAOPTS)
	! ./$< --equal 1.0.0 1.0   $(EXTRAOPTS)
	! ./$<         1.1   1.0   $(EXTRAOPTS)
	! ./$< --equal 1.1   1.0   $(EXTRAOPTS)
	! ./$<         1.1   1.0.0 $(EXTRAOPTS)
	! ./$< --equal 1.1   1.0.0 $(EXTRAOPTS)
	! ./$<         1.1.0 1.0   $(EXTRAOPTS)
	! ./$< --equal 1.1.0 1.0   $(EXTRAOPTS)
	  ./$<         1.0.A 1.0.a $(EXTRAOPTS)
	  ./$< --equal 1.0.A 1.0.a $(EXTRAOPTS)
	! ./$<         1.0.a 1.0.a $(EXTRAOPTS)
	  ./$< --equal 1.0.a 1.0.a $(EXTRAOPTS)
	! ./$<         1.0.a 1.0.A $(EXTRAOPTS)
	! ./$< --equal 1.0.a 1.0.A $(EXTRAOPTS)

clean:
	rm -f version-lt

