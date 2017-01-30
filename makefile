MAKE=$(shell which make)

build-src:
	$(MAKE) -C src -f makefile all

build-lib: build-src
	ar cr libdiffstats.a $(wildcard src/*.o)

build-tests: build-lib
	$(MAKE) -C tests -f makefile all

clean-src:
	$(MAKE) -C src -f makefile clean

clean-tests:
	$(MAKE) -C tests -f makefile clean

clean: clean-src clean-tests
	rm -rf libdiffstats.a

build: build-lib

run-tests:
	$(MAKE) -C tests -f makefile run

all: build-lib build-tests
