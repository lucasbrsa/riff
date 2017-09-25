.PHONY: clean run debug

# compiler and dirs
CC=clang
IDIR=../riff/.
SRC=main.c
BPROC=../build/main.test
BOBJ=../build/main.o
CFLAGS=-std=c99 -Wall -I$(IDIR)
LFLAGS=-lm

# compile
$(BOBJ): $(SRC)
	$(CC) -c -o $@ $< $(CFLAGS)

# link
$(BPROC): $(BOBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LFLAGS)

# run it
run: $(BPROC)
	@echo '-- RUNNING --'
	@$(BPROC)

debug: $(BPROC)
	lldb $(BPROC)

clean:
	rm -fv $(BPROC)
	rm -fv $(BOBJ)
