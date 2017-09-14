.PHONY: all riff clean tests run force debug

# from this file one may want to compile docs, src, tests...
# for now it's redundant; just the src (riff) dir

all: riff

riff:
	cd riff && $(MAKE)

run: riff
	cd riff && $(MAKE) $@

tests:
	cd tests && $(MAKE)

debug:
	cd riff && $(MAKE) $@

force:clean run

clean:
	cd riff && $(MAKE) $@
	cd tests && $(MAKE) $@
