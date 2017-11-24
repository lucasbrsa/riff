#include "test_error.h"
#include "test_hashmap.h"
#include "test_str.h"
#include "test_vector.h"
#include "test_log.h"
#include "test_mat.h"
#include "test.h"

int main() {
	/*test_run(error_test);
	test_run(hashmap_test);
	test_run(str_test);
	test_run(vector_test);
	test_run(log_test);*/
	
	test_run(mat_test);

	test_resolve();
}
