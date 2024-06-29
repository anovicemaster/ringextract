
export vend = /usr/local

all:
	$(MAKE) -C src

.PHONY: example
example:
	$(MAKE) -C example

.PHONY: clean
clean:
	$(MAKE) clean -C src
	