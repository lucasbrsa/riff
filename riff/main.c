#include "str.h"

#include <malloc.h>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

main() {
	char* buffer[100];
	clock_t start, end;
	double time=0, ctime;
	char* foo = "pure\0fcall";
	int i, j, k=0;
	lbl:
	for (j = 0; j < 20; j++) {
		start = clock();
		for (i = 0; i < 2097152; i++) {
			if (!k) str_stripf(&buffer[0], str_isspacec);
			else    str_strip(&buffer[0]);
		}
		end = clock();
		ctime = ((double)(end - start)) / CLOCKS_PER_SEC;
		printf("[%02i %s] %f\n", j+1, foo, ctime);
		time += ctime;
	}
	printf("av: %f\n", time / 20);
	if (!k++) {
		time = 0;
		foo += 5;
		goto lbl;
	}
	
	return getchar();
}