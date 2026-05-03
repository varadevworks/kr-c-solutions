CC := clang
CFLAGS := -Wall -Wextra -Wpedantic -std=c11 -g -O0

SRC_DIRS := exercises/chapter-01 exercises/chapter-02 exercises/chapter-03 exercises/chapter-04 exercises/chapter-05 exercises/chapter-06 exercises/chapter-07 exercises/chapter-08 exercises/chapter-09 exercises/chapter-10 exercises/chapter-11 exercises/chapter-12 exercises/chapter-13 exercises/chapter-14
VPATH := $(SRC_DIRS)
SRCS := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))
PROGRAMS := $(basename $(notdir $(SRCS)))
BINARIES := $(addprefix build/,$(PROGRAMS))

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
	$(CC) $(CFLAGS) $< -o $@

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
