#include "vector.h"

int main(int argc, char** argv) {
	vector_t* v = vector_init(0, sizeof(int));

	vector_push_back(v, 3498);
	vector_push_back(v, 3);
	vector_push_back(v, 6);
	vector_push_back(v, 10);
	vector_push_back(v, 15);
	vector_push_back(v, 21);
	vector_push_back(v, 28);

	vector_t* k = vector_copy(v);

	vector_pop_back(k);
	vector_set(k, 2, 45);
	vector_insert(k, 3, 865);
	vector_push_back(k, 70);

	vector_printi(v);
	vector_printi(k);

	vector_free(v); vector_free(k);

	return getchar();
}

/*
#include <malloc.h>
#include <stdio.h>
#include "log.h"

int main(int argc, char** argv) {
	log_fmt_t p = log_compile_pattern("[%c:%t] %% %m");
	log_msg_t* m = malloc(sizeof(log_msg_t));
	
	m->cat = 0;
	m->message = "smd";
	
	log_print_fmt(m, p);
	log_free_fmt(&p);
	free(m);

	return getchar();
}
*/