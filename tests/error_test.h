#include "test.h"
#include "error.h"

void error_test(void) {
	test_seq(error_get(), "no error");
	error_set("something's wrong...");

	test_seq(error_get(), "something's wrong...");
	error_code(ERROR_LOGICAL);

	test_seq(error_get(), "logical");

	error_clear();
}
