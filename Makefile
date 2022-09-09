# Comprehensive Makefile for CS261
# This will automatically compile any .c files in the current directory.
# Some of the syntax was adapted from:
# https://gist.github.com/Wenchy/64db1636845a3da0c4c7
# http://nuclear.mutantstargoat.com/articles/make/

CC = gcc
CFLAGS = --std=c99
exe_file = doit

# Handle debug case
DEBUG ?= 1
ifeq ($(DEBUG), 1)
	CFLAGS += -g -Wall
else
	CFLAGS += -DNDEBUG -O3
endif

SRCDIR = .
SRCEXT = c
SOURCES = $(shell find $(SRCDIR) -type f -name "*.$(SRCEXT)")
OBJECTS = $(SOURCES:.c=.o)
DEP = $(OBJECTS:.o=.d)

$(exe_file): $(OBJECTS)
	$(CC) $^ -o $(exe_file)

-include $(DEP)

%.d: %.$(SRCEXT)
	@$(CC) $(CFLAGS) $< -MM -MT $(@:.d=.o) >$@

%.o: %.$(SRCEXT)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.out *.o *.d $(exe_file)

tarball:
	tar -cvf assign4.tar *.h *.c Makefile

.PHONY: clean