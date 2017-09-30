.PHONY: all riff clean run force debug

all: riff

riff:
	cd riff && $(MAKE)

run: riff
	cd riff && $(MAKE) $@

debug:
	cd riff && $(MAKE) $@

force:clean run

clean:
	cd riff && $(MAKE) $@
