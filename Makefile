UNAME_S := $(shell uname -s)

ifeq ($(origin CC),default) # Only set CC if it hasn't been set in the environment or command line
ifeq ($(UNAME_S),Darwin) # on MacOS, use clang by default
CC := clang
else
CC := gcc # on other systems, use gcc by default like Linux
endif
endif

CFLAGS ?= -Wall -Wextra -Wpedantic -std=c11 -g -O0 # All warnings, C11 standard, debug symbols, no optimization
LDLIBS ?= -lm # Link with math library for exercises that use math functions. This is needed to run in Linux

SRC_DIRS := exercises/chapter-01 exercises/chapter-02 exercises/chapter-03 exercises/chapter-04 exercises/chapter-05 exercises/chapter-06 exercises/chapter-07 exercises/chapter-08
VPATH := $(SRC_DIRS) # Search for source files in these directories
SRCS := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c)) # Find all .c files in the source directories
PROGRAMS := $(basename $(notdir $(SRCS))) # Get program names by stripping directory and extension. E.g. ex-01-01, ex-02-01, etc.
BINARIES := $(addprefix build/,$(PROGRAMS)) # The corresponding binary names in the build/ directory

.PHONY: all help list run clean

all: $(BINARIES)

help:
	@printf "Usage:\n"
	@printf "  make                 Build all programs into build/\n"
	@printf "  make <name>          Build one program, e.g. make ex-14\n"
	@printf "  make run FILE=<name> Run one program, e.g. make run FILE=ex-14\n"
	@printf "  make run FILE=<name> INPUT=<path> Run with stdin from a file\n"
	@printf "  make list            Show available program names\n"
	@printf "  make clean           Remove build outputs\n"

list:
	@printf "%s\n" $(PROGRAMS)

build:
	@mkdir -p $@

build/%: %.c | build
	$(CC) $(CFLAGS) $< -o $@ $(LDLIBS)

$(PROGRAMS): %: build/%
	@true

run:
ifndef FILE
	$(error FILE is required. Example: make run FILE=ex-14)
endif
	@$(MAKE) --no-print-directory $(basename $(notdir $(FILE)))
ifdef INPUT
	@./build/$(basename $(notdir $(FILE))) < $(INPUT)
else
	@./build/$(basename $(notdir $(FILE)))
endif

clean:
	rm -rf build
