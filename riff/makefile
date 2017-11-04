.PHONY: clean run debug force

CC=clang
IDIR=-I.
SDIR=.
ODIR=../build

SRC := $(wildcard $(SDIR)/*.c)
OBJ := $(addprefix $(ODIR)/,$(notdir $(SRC:.c=.o)))

LFLAGS=-lm
CFLAGS=-std=c99 -g -Wall $(IDIR)

all: $(OBJ)

$(ODIR)/%.o: $(SDIR)/%.c
	$(CC) -c -o $@ $< $(CFLAGS)

force: clean all

clean:
	rm -fv $(ODIR)/*.o
