.PHONY: all clean run force

CC=clang
ODIR=../build/tests
LDIR=../build

OBJ := $(wildcard $(LDIR)/*.o)
EXE := $(patsubst %.c,$(ODIR)/%,$(wildcard *.c))

FLAGS=-std=c99 -g -Wall
LFLAGS= -L../riff $(OBJ) -lm -Wl,-rpath ../riff
IFLAGS=-I. -I../riff/

all: $(EXE)

$(EXE): %:%.o
	$(CC) $(FLAGS) -o $@ $^ $(LFLAGS)

$(ODIR)/%.o: %.c
	@mkdir -p $(ODIR)
	$(CC) $(FLAGS) -o $@ -c $< $(IFLAGS)

run: $(EXE)
	$(patsubst %, ./%;,$(EXE))

force: clean run

clean:
	rm -fv $(ODIR)/*
