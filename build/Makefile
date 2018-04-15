# Build Makefile
# ASA 2018
# Rafael Andrade, Gabriel Figueira
# P2

SHELL       := /bin/sh
PROGRAM     := ../asa.out

SRC_FILES   := ../src
LIB_FILES   := ../lib

CC          := gcc
CFLAGS      := -Wall -Wextra -pedantic -pedantic-errors -g
CFLAGS_R    := -O3 -ansi -Wall
LDFLAGS     := -lm
#CFLAGS      := $(CFLAGS_R)

basename    := main
objects      = $(addsuffix .o, $(basename))

vpath %.h $(SRC_FILES):$(LIB_FILES)
vpath %.c $(SRC_FILES):$(LIB_FILES)

define compile
	$(CC) $(CFLAGS) $(objects) -o $(PROGRAM) $(LDFLAGS)
endef

.PHONY: mooshak

mooshak     : basename = mooshak
mooshak     : mooshak.o
	$(compile)


# Source code


# Utils
.PHONY : clean cleaner full
clean   :
	$(RM) ./*.o

cleaner : clean
	$(RM) $(PROGRAM)

full    : cleaner $(PROGRAM)

