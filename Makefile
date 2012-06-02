# XmlDiff Makefile
# Copyright (C) 2012 Steve Sperandeo <steve dot sperandeo at gmail dot com>
# This file is released under the MIT license, see the COPYING file

uname_S := $(shell sh -c 'uname -s 2>/dev/null || echo not')
OPTIMIZATION?=-O2

CCCOLOR="\033[34m"
LINKCOLOR="\033[34;1m"
SRCCOLOR="\033[33m"
BINCOLOR="\033[37;1m"
MAKECOLOR="\033[32;1m"
ENDCOLOR="\033[0m"

ifndef V
QUIET_CC = @printf '    %b %b\n' $(CCCOLOR)CC$(ENDCOLOR) $(SRCCOLOR)$@$(ENDCOLOR);
QUIET_LINK = @printf '    %b %b\n' $(LINKCOLOR)LINK$(ENDCOLOR) $(BINCOLOR)$@$(ENDCOLOR);
endif

ifeq ($(uname_S),SunOS)
  CFLAGS?=-std=c99 -pedantic $(OPTIMIZATION) -Wall -W -D__EXTENSIONS__ -D_XPG6
  CCLINK?=-ldl -lnsl -lsocket -lm -lpthread
  DEBUG?=-g -ggdb
else
  CFLAGS?=-std=c99 -pedantic $(OPTIMIZATION) -Wall -W $(ARCH) $(PROF)
  CCLINK?=-lm -pthread
  DEBUG?=-g -rdynamic -ggdb
endif

CCOPT= $(CFLAGS) $(ARCH) $(PROF)

PREFIX?=/usr/local
INSTALL_BIN= $(PREFIX)/bin
INSTALL= cp -pf

OBJ = file.o token.o

PRGNAME = xmldiff

all: xmldiff
    @echo ""
    @echo "Hint: To run 'make test' is a good idea ;)"
    @echo ""

file.o: file.c file.h
token.o: token.c token.h

xmldiff: $(OBJ)
    $(QUIET_LINK)$(CC) -o $(PRGNAME) $(CCOPT) $(DEBUG) $(OBJ) $(CCLINK)

%.o: %.c
    $(QUIET_CC)$(CC) -c $(CFLAGS) $(DEBUG) $(COMPILE_TIME) $<

clean:
    rm -rf $(PRGNAME) *.o

log:
    git log '--pretty=format:%ad %s (%cn)' --date=short > ../Changelog

gprof:
    $(MAKE) PROF="-pg"

gcov:
    $(MAKE) PROF="-fprofile-arcs -ftest-coverage"

install: all
    mkdir -p $(INSTALL_BIN)
    $(INSTALL) $(PRGNAME) $(INSTALL_BIN)
