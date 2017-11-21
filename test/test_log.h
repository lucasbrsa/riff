#include "test.h"
#include "logging.h"

#include <stdio.h>

void log_test(void) {
	FILE* flog = fopen("res/test.log", "w");
	logging_set_file(flog);

	logging_set_silent();

	int count = logging_get_count();
	logging_set_level(LOGGING_WARN);
	logging_debug("example");
	logging_info("example");
	logging_notice("example");
	logging_warn("example");
	logging_error("example");
	logging_panic("example");

	test_assert(logging_get_count() - count == 3);

	fseek(flog, 0L, SEEK_END);
	test_assert(ftell(flog) != 0);

	fclose(logging_get_file());
}
