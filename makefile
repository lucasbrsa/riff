.PHONY: all riff tests clean run force debug

all: tests

riff:
	@cd riff && $(MAKE)

tests:
	@cd riff && $(MAKE) force
	@cd tests && $(MAKE) force

run: riff
	@cd riff && $(MAKE) $@

debug:
	@cd riff && $(MAKE) $@

force:clean run

clean:
	@cd riff && $(MAKE) $@
