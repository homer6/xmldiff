# XmlDiff Makefile
# Copyright (C) 2012 Steve Sperandeo <steve dot sperandeo at gmail dot com>
# This file is released under the MIT license, see the COPYING file

uname_S := $(shell sh -c 'uname -s 2>/dev/null || echo not')
OPTIMIZATION?=-O0

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
TEST_DIR=./test/
BIN_DIR=./bin/

OBJ = $(BIN_DIR)token.o $(BIN_DIR)error.o $(BIN_DIR)debug.o $(BIN_DIR)array.o $(BIN_DIR)xml_element.o $(BIN_DIR)xml_parser.o $(BIN_DIR)file.o
TEST_OBJ = $(BIN_DIR)array_test.o $(BIN_DIR)xml_element_test.o $(BIN_DIR)xml_parser_test.o $(BIN_DIR)file_test.o

PROGRAM_NAME = xmldiff
TEST_PROGRAM_NAME = xmldiff_test

all: xmldiff xmldiff_test

#$(BIN_DIR)file.o: $(SOURCE_DIR)file.c $(SOURCE_DIR)file.h
#$(BIN_DIR)token.o: $(SOURCE_DIR)token.c $(SOURCE_DIR)token.h

xmldiff: $(OBJ)
	$(CC) -o $(PROGRAM_NAME) $(CCOPT) $(DEBUG) $(OBJ) $(CCLINK) $(SOURCE_DIR)main.c

xmldiff_test: $(OBJ) $(TEST_OBJ)
	$(CC) -o $(TEST_PROGRAM_NAME) $(CCOPT) $(DEBUG) $(OBJ) $(TEST_OBJ) $(CCLINK) $(TEST_DIR)main.c

#%.o: %.c
#	$(CC) -c $(CFLAGS) $(DEBUG) $(COMPILE_TIME) -c $(input) -o $(output)

	
$(BIN_DIR)token.o:
	$(CC) $(CCOPT) $(DEBUG) $(CCLINK) -c $(SOURCE_DIR)token.c -o $(BIN_DIR)token.o

	
$(BIN_DIR)error.o:
	$(CC) $(CCOPT) $(DEBUG) $(CCLINK) -c $(SOURCE_DIR)error.c -o $(BIN_DIR)error.o

	
$(BIN_DIR)debug.o:
	$(CC) $(CCOPT) $(DEBUG) $(CCLINK) -c $(SOURCE_DIR)debug.c -o $(BIN_DIR)debug.o

	
$(BIN_DIR)array.o:
	$(CC) $(CCOPT) $(DEBUG) $(CCLINK) -c $(SOURCE_DIR)array.c -o $(BIN_DIR)array.o
	
$(BIN_DIR)array_test.o:
	$(CC) $(CCOPT) $(DEBUG) $(CCLINK) -c $(TEST_DIR)array_test.c -o $(BIN_DIR)array_test.o

	
$(BIN_DIR)xml_element.o:
	$(CC) $(CCOPT) $(DEBUG) $(CCLINK) -c $(SOURCE_DIR)xml_element.c -o $(BIN_DIR)xml_element.o
	
$(BIN_DIR)xml_element_test.o:
	$(CC) $(CCOPT) $(DEBUG) $(CCLINK) -c $(TEST_DIR)xml_element_test.c -o $(BIN_DIR)xml_element_test.o


$(BIN_DIR)xml_parser.o:
	$(CC) $(CCOPT) $(DEBUG) $(CCLINK) -c $(SOURCE_DIR)xml_parser.c -o $(BIN_DIR)xml_parser.o
	
$(BIN_DIR)xml_parser_test.o:
	$(CC) $(CCOPT) $(DEBUG) $(CCLINK) -c $(TEST_DIR)xml_parser_test.c -o $(BIN_DIR)xml_parser_test.o


$(BIN_DIR)file.o:
	$(CC) $(CCOPT) $(DEBUG) $(CCLINK) -c $(SOURCE_DIR)file.c -o $(BIN_DIR)file.o
	
$(BIN_DIR)file_test.o:
	$(CC) $(CCOPT) $(DEBUG) $(CCLINK) -c $(TEST_DIR)file_test.c -o $(BIN_DIR)file_test.o
	
	

	
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
