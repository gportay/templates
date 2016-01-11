#
# Copyright 2016 Gaël PORTAY <gael.portay@gmail.com>
#
# Licensed under the MIT license.
#

CFLAGS	+= -Werror -Wall -Wextra

.SILENT: run-test test

VERCMP	?= version-lt
all: $(VERCMP)

version-lt:

test: $(VERCMP)
	  ./$(VERCMP)         0.9   1.0   $(EXTRAOPTS)
	  ./$(VERCMP) --equal 0.9   1.0   $(EXTRAOPTS)
	  ./$(VERCMP)         0.9   1.0.0 $(EXTRAOPTS)
	  ./$(VERCMP) --equal 0.9   1.0.0 $(EXTRAOPTS)
	  ./$(VERCMP)         0.9   1.0.0 $(EXTRAOPTS)
	  ./$(VERCMP) --equal 0.9   1.0.0 $(EXTRAOPTS)
	  ./$(VERCMP)         0.9.0 1.0   $(EXTRAOPTS)
	  ./$(VERCMP) --equal 0.9.0 1.0   $(EXTRAOPTS)
	! ./$(VERCMP)         1.0   1.0   $(EXTRAOPTS)
	  ./$(VERCMP) --equal 1.0   1.0   $(EXTRAOPTS)
	  ./$(VERCMP)         1.0   1.0.0 $(EXTRAOPTS)
	  ./$(VERCMP) --equal 1.0   1.0.0 $(EXTRAOPTS)
	! ./$(VERCMP)         1.0.0 1.0   $(EXTRAOPTS)
	! ./$(VERCMP) --equal 1.0.0 1.0   $(EXTRAOPTS)
	! ./$(VERCMP)         1.1   1.0   $(EXTRAOPTS)
	! ./$(VERCMP) --equal 1.1   1.0   $(EXTRAOPTS)
	! ./$(VERCMP)         1.1   1.0.0 $(EXTRAOPTS)
	! ./$(VERCMP) --equal 1.1   1.0.0 $(EXTRAOPTS)
	! ./$(VERCMP)         1.1.0 1.0   $(EXTRAOPTS)
	! ./$(VERCMP) --equal 1.1.0 1.0   $(EXTRAOPTS)
	  ./$(VERCMP)         1.0.A 1.0.a $(EXTRAOPTS)
	  ./$(VERCMP) --equal 1.0.A 1.0.a $(EXTRAOPTS)
	! ./$(VERCMP)         1.0.a 1.0.a $(EXTRAOPTS)
	  ./$(VERCMP) --equal 1.0.a 1.0.a $(EXTRAOPTS)
	! ./$(VERCMP)         1.0.a 1.0.A $(EXTRAOPTS)
	! ./$(VERCMP) --equal 1.0.a 1.0.A $(EXTRAOPTS)

run-test:
	echo "Testing c version..."
	make -f Makefile test
	echo "Testing shell version..."
	make -f Makefile test VERCMP="version-lt.sh"

clean:
	rm -f version-lt

