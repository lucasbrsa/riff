#include "error_test.h"
#include "hashmap_test.h"
#include "log_test.h"
#include "os_test.h"
#include "str_test.h"
#include "vector_test.h"

#include "test.h"

int main() {
	test_run(error_test);
	test_run(hashmap_test);
	test_run(log_test);
	test_run(os_test);
	test_run(str_test);
	test_run(vector_test);

	test_resolve();
}
