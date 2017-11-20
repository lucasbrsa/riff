.PHONY: all build test debug_test clean rebuild valgrind

MFLAGS=--no-print-directory

all: build

build:
	@cd riff && $(MAKE) $(MFLAGS)

test:build
	@cd test && $(MAKE) $(MFLAGS) run

debug_test:build
	@cd test && $(MAKE) $(MFLAGS) debug

valgrind:build
	@cd test && $(MAKE) $(MFLAGS) valgrind

clean:
	@cd riff && $(MAKE) $(MFLAGS) $@
	@cd test && $(MAKE) $(MFLAGS) $@

rebuild: clean build
