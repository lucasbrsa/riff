.PHONY: all build test clean rebuild

MFLAGS=--no-print-directory

all: build

build:
	@cd riff && $(MAKE) $(MFLAGS)

test:build
	@cd test && $(MAKE) $(MFLAGS) run

clean:
	@cd riff && $(MAKE) $(MFLAGS) $@
	@cd test && $(MAKE) $(MFLAGS) $@

rebuild: clean build
