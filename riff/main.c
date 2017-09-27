#include <assert.h>
#include <stdio.h>
#include <time.h>

#include "log.h"

void other_function(void) {
	if (log_logger_get("test logger"))
		puts("i love hashmaps");
}

int main(int argc, char ** argv) {

	log_logger_t* my_logger = log_logger_init("test logger", NULL);

	assert(my_logger != NULL);
	puts("created logger");

	if (my_logger == log_logger_get("test logger"))
		puts("the hashmap seems the be working...");

	other_function();

	log_logger_free(my_logger);
	puts("destroyed logger");

	other_function();

	log_free();
	other_function();

	return 0;
}
