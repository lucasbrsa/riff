#include "test.h"
#include "log.h"

#include <stdio.h>

void log_test(void) {
	FILE* flog = fopen("res/test.log", "w");
	log_set_file(flog);

	log_set_silent();

	int count = log_get_count();
	log_set_level(LOG_LEVEL_WARN);
	log_debug("example");
	log_info("example");
	log_notice("example");
	log_warn("example");
	log_error("example");
	log_panic("example");

	test_assert(log_get_count() - count == 3);

	fseek(flog, 0L, SEEK_END);
	test_assert(ftell(flog) != 0);

	fclose(log_get_file());
}
