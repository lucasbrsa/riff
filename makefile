.PHONY: all riff clean tests run force debug

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
