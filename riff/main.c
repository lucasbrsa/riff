#include "str.h"

#include <stdlib.h>
#include <time.h>
#include <stdio.h>

main() { 
	double time_spent = 0.0;
	char buffer[__UI64_MAXS];
	for (int j = 0; j < 10; j++) {
		for (int i = 0; i < 1000000; i++) {
			clock_t begin = clock();

			//str_uits(i, buffer);
			//sprintf(buffer, "%i", i);
			itoa(i, buffer, 10);

			clock_t end = clock();
			time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
		}
		printf("[ITOA] %f\n", time_spent);
		time_spent = 0;
	}

	for (int j = 0; j < 10; j++) {
		for (int i = 0; i < 1000000; i++) {
			clock_t begin = clock();

			str_uits(i, buffer);

			clock_t end = clock();
			time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
		}
		printf("[str_uits] %f\n", time_spent);
		time_spent = 0;
	}

	return getchar();
}