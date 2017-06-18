#if 0
#include "logging.h"
#include "logdef.h"

main() {
	log_slt_open(fopen("../resources/proc.txt", "w"), NULL);
	log_slt_open(stdout, NULL);

	log_trace(LCAPP, "log");
	log_debug(LCAPP, "log");
	log_info(LCAPP, "log");
	log_warn(LCAPP, "log");
	log_error(LCAPP, "log");
	log_fatal(LCAPP, "log");

	log_cap_set(LPERR, LCREND);

	log_trace(LCREND, "log");
	log_debug(LCREND, "log");
	log_info(LCREND, "log");
	log_warn(LCREND, "log");
	log_error(LCREND, "log");
	log_fatal(LCREND, "log");

	log_quit();
	getchar();
}
#endif

#include <stdio.h>
#include <string.h>

int power(int base, int exponent)
{
	if (!exponent) { return 1; }
	int result = base;
	for (int i = 1; i < exponent; i++) { result *= base; }
	return result;
}

int get_hashcode(const char* str, size_t len)
{
	int hashed = NULL;

	for (int i = 0; i < len; i++) { hashed += (int)str[i] * power(31, len - (i + 1)); }

	return hashed % 256;
}


int main(int argc, char** argv)
{

	const char* test = " ";

	printf("\nHashcode: %i\n", get_hashcode(test, strlen(test)));
	

	printf("ruchir");

	
	getchar(); return 0;
}