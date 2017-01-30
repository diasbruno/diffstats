CX=clang
CXX=$(CX)++

CFLAGS=-Wall -Werror -std=c99 -g

SOURCES=$(wildcard *.c)
OBJECTS=$(SOURCES:%.c=%.o)

INCLUDES=-I../include

include ../tools/helpers.mk

all: $(OBJECTS)

clean:
	rm -rf $(OBJECTS)
