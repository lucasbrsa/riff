.PHONY: clean run debug force

# compiler and dirs
CC=clang
IDIR=-I.
SDIR=.
ODIR=../build
BPROC=$(ODIR)/riff

# files
SRC := $(wildcard $(SDIR)/*.c)
OBJ := $(addprefix $(ODIR)/,$(notdir $(SRC:.c=.o)))

# flags
LFLAGS=-lm
CFLAGS=-std=c99 -g -Wall $(IDIR)

# compile
$(ODIR)/%.o: $(SDIR)/%.c
	$(CC) -c -o $@ $< $(CFLAGS)

# link
$(BPROC): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LFLAGS)

# run it
run: $(BPROC)
	@echo '-- RUNNING --'
	@$(BPROC)

# run with debugger
debug: $(BPROC)
	lldb $(BPROC)

force: clean run

# get rid of all the garbage
clean:
	rm -fv $(BPROC)
	rm -fv $(ODIR)/*.o
