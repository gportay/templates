PREFIX ?= /usr/local

.PHONY: all
all: ini-parser.bash.7.gz

.PHONY: man
man: ini-parser.bash.7.gz

.PHONY: install
install:
	install -d $(DESTDIR)$(PREFIX)/libexec/bash/
	install -m 644 ini-parser.bash $(DESTDIR)$(PREFIX)/libexec/bash/
	install -d $(DESTDIR)$(PREFIX)/share/man/man7/
	install -m 644 ini-parser.bash.7.gz $(DESTDIR)$(PREFIX)/share/man/man7/

.PHONY: tests
tests:
	@bash --version
	@for test in $@/[0-9][0-9]*.bash; do \
		echo -e "\e[1mRunning $${test##*/}...\e[0m"; \
		if ! $$test ini-parser.bash; then \
			echo -e "\e[1m$${test##*/}: \e[31m[KO]\e[0m"; \
		else \
			echo -e "\e[1m$${test##*/}: \e[32m[OK]\e[0m"; \
		fi; \
		echo; \
	done

.PHONY: clean
clean:
	rm -f ini-parser.bash.7.gz

%.7: %.7.adoc
	asciidoctor -b manpage -o $@ $<

%.gz: %
	gzip -c $^ >$@

