.PHONY: all clean run force debug

CC=clang
ODIR=../build
LDIR=../riff

OBJ := $(patsubst $(LDIR)/%.c,$(ODIR)/%.o,$(wildcard $(LDIR)/*.c))
EXE := $(patsubst %.c,%,$(wildcard *.c))

FLAGS=-std=c99 -g -Wall
LFLAGS= -L$(LDIR) $(OBJ) -lm -Wl,-rpath $(LDIR)
IFLAGS=-I. -I$(LDIR)

all: $(EXE)

$(EXE): %:%.o
	$(CC) $(FLAGS) -o $@ $^ $(LFLAGS)

.c.o:
	$(CC) $(FLAGS) -o $@ -c $< $(IFLAGS)

run: $(EXE)
	@$(patsubst %,echo "--- RUNNING ---";./%;,$(EXE))

debug:
	@lldb

force: clean run

clean:
	rm -fv *.o $(EXE)
