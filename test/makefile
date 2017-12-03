.PHONY: clean build_% run_% debug_% rebuild all valgrind_%

CC=gcc
ODIR=../build
LDIR=..
LRIFF=$(ODIR)/libriff.a
FLAGS=-std=c99 -g -Wall
LFLAGS= -L../build -lriff -lm -lX11 -Wl,-rpath ../build
IFLAGS=-I. -I../include
VFLAGS=--track-origins=yes --leak-check=full

HDRS := $(wildcard *.h)
BPROC := $(patsubst %.c,$(ODIR)/%,$(wildcard *.c))

all: $(BPROC)

$(LRIFF):
	cd $(LDIR) && $(MAKE)

$(ODIR)/%: %.c $(LRIFF) $(HDRS)
	$(CC) $(FLAGS) -o $@ $< $(IFLAGS) $(LFLAGS)

build_%: $(ODIR)/%
	@echo '** BUILT **'

run_%: $(ODIR)/%
	@echo '** RUNNING **'
	@./$<

debug_%: $(ODIR)/%
	@echo '** DEBUGGING **'
	@gdb $<

valgrind_%: $(ODIR)/%
	@valgrind $(VFLAGS) $<

clean:
	@# valgrind files...
	@rm -fv $(BPROC)

rebuild: clean run
