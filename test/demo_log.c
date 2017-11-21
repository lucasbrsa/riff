#ifdef OLD_LOGGING
#define LOG_LEVEL 3

#include "log.h"

int main() {
	log_logger_t* console = log_logger("console", log_writer_stdout());

	log_notice(console, "Hello %s", "world!");
	log_notice(console, "I am a simple stdout logger");

	log_warn_if(1 == 0, console, "Oh no...");
	log_info(log_logger_get("console"), "You can find a logger by name.");

	log_panic(console, "I'm suiciding now...");
	log_logger_free(console);

	log_logger_t* rainbow = log_logger("rainbow", log_writer_stdout_coloured());

	log_fmt_set("[%t] [%n] [%p] %m");

	log_debug(rainbow, "hello");
	log_info(rainbow, "hello");
	log_notice(rainbow, "hello");
	log_warn(rainbow, "hello");
	log_error(rainbow, "hello");
	log_panic(rainbow, "hello");

	log_logger_t* syslogger = log_logger("syslogger", log_writer_syslog(0));
	log_notice(syslogger, "This was written through %s", "log.h");

	LOG_DEBUG(rainbow, "This line of code never ran...");
	LOG_INFO(rainbow, "This line of code never ran...");
	LOG_NOTICE(rainbow, "This line of code ran...");

	log_free();

	log_logger_t* general_logger = log_logger("general", log_writer_null());
	log_logger_add_writer(general_logger, log_writer_stdout_coloured());
	log_logger_add_writer(general_logger, log_writer_syslog(0));
	log_logger_add_writer(general_logger, log_writer_file("../res/general.log"));

	log_notice(log_logger_get("general"), "This was created by a multi-writer logger");
}
#endif

#include "logging.h"

int main() {
	logging_set_writer(logging_writer_stdout);

	logging_notice("Hello %s", "world!");
	logging_notice("I am an stdout writer");

	logging_warn_if(1 == 0, "Oh no...");
	/* loggers are a lame idea */

	logging_panic("I'm suiciding now");

	/* there aren't separate loggers, just one state machine */
	/* just uses a callback function called a 'writer' */
	logging_set_writer(logging_writer_coloured);

	/* you can't change the format, just implement your own writer */
	logging_debug("hello");
	logging_info("hello");
	logging_notice("hello");
	logging_warn("hello");
	logging_error("hello");
	logging_panic("hello");

	/* basically all you ever need */
	logging_set_writer(logging_writer_colourful_and_file);
	logging_set_file(fopen("res/demo.log", "w"));

	logging_notice("this is also written to a file");

	logging_set_silent();

	logging_notice("this only goes to the file and no stderr");

	logging_set_noisy();

	logging_warn("ending demo");

	fclose(logging_get_file());
}

