all: strtonargv argvtonstr

argv: override CFLAGS+=-Wall -Wextra -Werror -g
argv.o: argv.h

strtonargv argvtonstr: argv
	ln -sf $< $@

.PHONY: test
test: PATH:=.:$(PATH)
test: SHELL=/bin/bash
test: all
	printf "%s\0%s\0%s\0%s" "-sh" "-c" "echo whoami" | hexdump -C | \
	diff - <(argvtonstr -- "-sh" "-c" "echo whoami" | hexdump -C)
	printf "%s\0%s\0%s\0%s" "-sh" "-c" "echo whoami" | hexdump -C
	printf "%s %s %s %s\n" "-sh" "-c" "echo whoami" | hexdump -C | \
	diff - <(printf "%s\0%s\0%s\0%s" "-sh" "-c" "echo whoami" | strtonargv | hexdump -C)

.SILENT: debug
.PHONY: debug
debug: PATH:=.:$(PATH)
debug: all
	valgrind argvtonstr -- "-sh" "-c" "echo whoami"

.PHONY: clean
clean:
	rm -f argv strtonargv argvtonstr *.o

# filetype=makefile
