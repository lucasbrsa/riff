#include "error.h"
#include "log.h"

int main() {
	error_logger(log_logger("error log", log_writer_stderr()));
	log_fmt_set("[%f:%F:%l] %m");

	error_code(ERROR_LOGICAL);
	error_set("right thruster down");
	error_set("fire in main cabin");
	error_set("navigation system down");
	error_set("unexpected system crash..");

	error_log();

	log_free();
	error_clear();
}
