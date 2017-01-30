CX=clang
CXX=$(CX)++

CFLAGS=-Wall -Werror -std=c99

SOURCES=$(wildcard *.c)
OBJECTS=$(SOURCES:%.c=%.o)

INCLUDES=-I ../include

include ../tools/helpers.mk

tests: $(OBJECTS)
	$(CX) -MMD -MP -MF $@.d $(CFLAGS) $(INCLUDES) -o $@ $^ ../libdiffstats.a

-include tests.d

run:
	./tests

all: tests

clean:
	rm -rf $(OBJECTS) tests
