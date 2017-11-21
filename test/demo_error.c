#include "error.h"

int main() {
	error_set_silent(false);

	error_code(ERROR_LOGICAL);
	error_set("right thruster down");
	error_set("fire in main cabin");
	error_set("navigation system down");
	error_set("unexpected system crash..");

	error_clear();
}
