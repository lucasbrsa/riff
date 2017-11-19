#include "test.h"
#include "logging.h"

void log_test(void) {
	logging_debug("example");
	logging_info("example");
	logging_notice("example");
	logging_warn("example");
	logging_error("example");
	logging_panic("example");
}
