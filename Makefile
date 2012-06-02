# XmlDiff Makefile
# Copyright (C) 2012 Steve Sperandeo <steve dot sperandeo at gmail dot com>
# This file is released under the MIT license, see the COPYING file

uname_S := $(shell sh -c 'uname -s 2>/dev/null || echo not')
OPTIMIZATION?=-O2

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

SOURCE_DIR=./src/
BIN_DIR=./bin/

OBJ = $(BIN_DIR)file.o $(BIN_DIR)token.o

PRGNAME = xmldiff

all: xmldiff

#$(BIN_DIR)file.o: $(SOURCE_DIR)file.c $(SOURCE_DIR)file.h
#$(BIN_DIR)token.o: $(SOURCE_DIR)token.c $(SOURCE_DIR)token.h

xmldiff: $(OBJ)
	$(CC) -o $(PRGNAME) $(CCOPT) $(DEBUG) $(OBJ) $(CCLINK) $(SOURCE_DIR)main.c

#%.o: %.c
#	$(CC) -c $(CFLAGS) $(DEBUG) $(COMPILE_TIME) -c $(input) -o $(output)

$(BIN_DIR)file.o:
	$(CC) $(CCOPT) $(DEBUG) $(CCLINK) -c $(SOURCE_DIR)file.c -o $(BIN_DIR)file.o
	
$(BIN_DIR)token.o:
	$(CC) $(CCOPT) $(DEBUG) $(CCLINK) -c $(SOURCE_DIR)token.c -o $(BIN_DIR)token.o



clean:
	rm -rf $(PRGNAME) $(BIN_DIR)*.o

log:
	git log '--pretty=format:%ad %s (%cn)' --date=short > ../Changelog

gprof:
	$(MAKE) PROF="-pg"

gcov:
	$(MAKE) PROF="-fprofile-arcs -ftest-coverage"

install: all
	mkdir -p $(INSTALL_BIN)
	$(INSTALL) $(PRGNAME) $(INSTALL_BIN)
