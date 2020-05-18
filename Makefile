.PHONY: all
all:

.PHONY: clean
clean:
	-for subdir in c; do
		$(MAKE) -C $$subdir; \
	done
