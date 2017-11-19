.PHONY: clean run debug rebuild all

CC=clang
ODIR=../build
BPROC=$(ODIR)/test
LRIFF=$(ODIR)/libriff.a

FLAGS=-std=c99 -g -Wall
LFLAGS= -L../build -lriff -lm -Wl,-rpath ../build
IFLAGS=-I. -I../riff/

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

clean:
	@rm -fv $(BPROC)

rebuild: clean run
