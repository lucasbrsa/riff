#include "test.h"
#include "os.h"

void os_test(void) {
	OS_MKDIR("some_dir");
	OS_RENAME("some_dir", "my_dir");
	OS_RMDIR("my_dir");
}
