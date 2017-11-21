.PHONY: clean run debug rebuild all valgrind

CC=clang
ODIR=../build
LDIR=../riff
LRIFF=$(LDIR)/libriff.a
FLAGS=-std=c99 -g -Wall
LFLAGS= -L../build -lriff -lm -Wl,-rpath ../build
IFLAGS=-I. -I../riff/
VFLAGS=--track-origins=yes --leak-check=full

HDRS := $(wildcard *.h)
BPROC := $(patsubst %.c,$(ODIR)/%,$(wildcard *.c))

all: $(BPROC)

$(LRIFF):
	cd $(LDIR) && $(MAKE)

$(ODIR)/%: %.c $(LRIFF) $(HDRS)
	$(CC) $(FLAGS) -o $@ $< $(IFLAGS) $(LFLAGS)

run_%: $(ODIR)/%
	@echo '** RUNNING **'
	@./$<

debug_%: $(ODIR)/%
	@echo '** DEBUGGING **'
	@lldb $<

valgrind_%: $(ODIR)/%
	@valgrind $(VFLAGS) $<

clean:
	@# valgrind files...
	@rm -fv $(BPROC)

rebuild: clean run
