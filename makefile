.PHONY: all build test debug_test clean rebuild valgrind

MFLAGS=--no-print-directory

all: build

build:
	@cd riff && $(MAKE) $(MFLAGS)

test:build
	@cd test && $(MAKE) $(MFLAGS) run_test

debug_test:build
	@cd test && $(MAKE) $(MFLAGS) debug_test

valgrind:build
	@cd test && $(MAKE) $(MFLAGS) valgrind_test

clean:
	@cd riff && $(MAKE) $(MFLAGS) $@
	@cd test && $(MAKE) $(MFLAGS) $@

rebuild: clean build
