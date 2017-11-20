.PHONY: clean run debug rebuild all valgrind

CC=clang
ODIR=../build
BPROC=$(ODIR)/test
LRIFF=$(ODIR)/libriff.a

FLAGS=-std=c99 -g -Wall
LFLAGS= -L../build -lriff -lm -Wl,-rpath ../build
IFLAGS=-I. -I../riff/
VFLAGS=--track-origins=yes --leak-check=full

EXE=test.c
HDRS := $(wildcard *.h)

all: $(BPROC)

$(LRIFF):
	cd $(ODIR) && $(MAKE)

$(BPROC): $(EXE) $(LRIFF) $(HDRS)
	$(CC) $(FLAGS) -o $@ $< $(IFLAGS) $(LFLAGS)

run: $(BPROC)
	@echo '** RUNNING **'
	@./$(BPROC)

debug: $(BPROC)
	@echo '** DEBUGGING **'
	@lldb $(BPROC)

valgrind: $(BPROC)
	@valgrind $(VFLAGS) $(BPROC)

clean:
	@rm -fv $(BPROC)

rebuild: clean run
