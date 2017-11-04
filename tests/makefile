.PHONY: all clean run force debug

CC=clang
ODIR=../build
BPROC=$(ODIR)/tests

OOBJ := $(wildcard $(ODIR)/*.o)
SRC := $(wildcard *c)
MOBJ := $(addprefix $(ODIR)/,$(notdir $(SRC:.c=.o)))

FLAGS=-std=c99 -g -Wall
LFLAGS= -L../riff $(OOBJ) -lm -Wl,-rpath ../riff
IFLAGS=-I. -I../riff/

all: $(BPROC)

$(BPROC): $(MOBJ)
	$(CC) $(FLAGS) -o $@ $^ $(LFLAGS)

$(ODIR)/%.o: %.c
	$(CC) $(FLAGS) -o $@ -c $< $(IFLAGS)

run: $(BPROC)
	@echo "--- RUNNING ---"
	@./$(BPROC)

debug: $(BPROC)
	@lldb $(BPROC)

force: clean run

clean:
	rm -fv $(MOBJ)
	rm -fv $(BPROC)
