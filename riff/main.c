//	NEED TO FIX THE VECTOR DELETION!!!!

#include <malloc.h>

// #include "log.h"
#include "vector.h"

int main(int argc, char ** argv) {
	vector_t* my_vec = vector_init(128, sizeof(int*), &free);

	int* a = malloc(sizeof(int)); *a = 4;
	int* b = malloc(sizeof(int)); *b = 7;

	printf("*i = %i; i = %p; &i = %p\n", *a, a, &a);
	printf("*i = %i; i = %p; &i = %p\n", *b, b, &b);

	vector_push_back(my_vec, &a);
	vector_push_back(my_vec, &b);

	vector_free(my_vec);

	// log_fmt_t* f = log_compile_pattern("here is a very simple %m pattern...");
	// log_free_pattern(f);
}
