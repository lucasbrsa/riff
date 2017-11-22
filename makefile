.PHONY: list run_% debug_% valgrind_% clean build rebuild all run test debug valgrind force demo_% tag ctags

MFLAGS=--no-print-directory
MAKE=make $(MFLAGS)

CC=clang
SDIR=src
ODIR=build
BPROC=$(ODIR)/libriff.a

SRC := $(wildcard $(SDIR)/*.c)
OBJ := $(patsubst $(SDIR)/%.c,$(ODIR)/%.o,$(SRC))

LFLAGS=-cvq
CFLAGS=-std=c99 -g -Wall -Iinclude/

$(ODIR)/%.o: $(SDIR)/%.c
	$(CC) -c -o $@ $< $(CFLAGS)

$(BPROC): $(OBJ) $(SRC)
	ar $(LFLAGS) $(BPROC) $(OBJ)

list: $(BPROC)
	@ar -t $(BPROC)

run_%:build
	@cd test && $(MAKE) $@

debug_%:build
	@cd test && $(MAKE) $@

valgrind_%:build
	@cd test && $(MAKE) $@

clean:
	@rm -fv $(BPROC) $(ODIR)/*.o
	@cd test && $(MAKE) $@

build: $(BPROC)

rebuild: clean build

all: build

run: run_test

test: run_test

debug: debug_test

valgrind: valgrind_test

force: rebuild

demo_%:
	$(MAKE) run_$@

tag:
	ctags --recurse .

ctags: tag
