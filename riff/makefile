.PHONY: clean rebuild all list

CC=clang
SDIR=.
ODIR=../build
BPROC=$(ODIR)/libriff.a

SRC := $(wildcard $(SDIR)/*.c)
OBJ := $(addprefix $(ODIR)/,$(notdir $(SRC:.c=.o)))

LFLAGS=-cvq
CFLAGS=-std=c99 -g -Wall -I.

all: $(BPROC)

$(ODIR)/%.o: $(SDIR)/%.c
	$(CC) -c -o $@ $< $(CFLAGS)

$(BPROC): $(OBJ)
	ar $(LFLAGS) $(BPROC) $(OBJ)

list: $(BPROC)
	@ar -t $(BPROC)

clean:
	@rm -fv $(BPROC) $(ODIR)/*.o

rebuild: clean all
