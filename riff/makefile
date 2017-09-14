.PHONY: clean run

# compiler and dirs
CC=clang
IDIR=.
SDIR=.
ODIR=../build
BPROC=$(ODIR)/riff

# files
SRC := $(wildcard $(SDIR)/*.c)
OBJ := $(addprefix $(ODIR)/,$(notdir $(SRC:.c=.o)))

# flags
LFLAGS=-lm
CFLAGS=-std=c99 -Wall -I$(IDIR)

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

# get rid of all the garbage
clean:
	rm -fv $(BPROC)
	rm -fv $(ODIR)/*.o
