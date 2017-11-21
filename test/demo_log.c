#include "log.h"

int main() {
	log_set_writer(log_writer_stdout);

	log_notice("Hello %s", "world!");
	log_notice("I am an stdout writer");

	log_warn_if(1 == 0, "Oh no...");

	log_set_writer(log_writer_coloured);

	log_debug("hello");
	log_info("hello");
	log_notice("hello");
	log_warn("hello");
	log_error("hello");
	log_panic("hello");

	log_set_writer(log_writer_colourful_and_file);
	log_set_file(fopen("res/demo.log", "w"));

	log_notice("this is also written to a file");

	log_set_silent();

	log_notice("this only goes to the file and no stderr");

	log_set_noisy();

	log_warn("ending demo");

	fclose(log_get_file());
}

