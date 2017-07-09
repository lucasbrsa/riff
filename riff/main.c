#include "str.h"
#include <string.h>

#define test_str "Celina Pang!!!"

main() { 
	char* buf = malloc(100);
	memcpy(buf, test_str, sizeof(test_str) + 1);
	puts(str_swap_case(buf));
	puts(str_tolower(buf));
	puts(str_toupper(buf));

	free(buf);

	return getchar();
}