#define LOG_LEVEL 3

#include "test.h"
#include "log.h"

void log_test(void) {
	return;
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

int main() { test_run(log_test); test_resolve(); }
