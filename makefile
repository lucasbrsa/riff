.PHONY: all riff tests clean run force debug

all: tests

riff:
	@cd riff && $(MAKE)

tests:riff
	@cd tests && $(MAKE) run

debug:riff
	@cd tests && $(MAKE) $@

force:clean
	@cd riff && $(MAKE) force
	@cd tests && $(MAKE) force

clean:
	@cd riff && $(MAKE) $@
	@cd tests && $(MAKE) $@
