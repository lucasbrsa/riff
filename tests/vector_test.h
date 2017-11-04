#include "test.h"
#include "vector.h"

#include <string.h>

typedef struct {
	int radius, mass, x, y;
} vec_example_t;

void vector_test(void) {
	vector_t* m = vector_init(8, sizeof(char*), NULL);

	char* appends[] = { "to", "interject", "for", "a" };

	vector_push_back(m, "Richard");
	vector_push_back(m, "like");
	vector_insert(m, 1, "just");
	vector_insert(m, 1, "would");

	vector_append(m, appends, sizeof(appends) / sizeof(*appends));

	vector_reserve(m, 2); /* will do nothing */
	vector_reserve(m, 128);

	vector_push_back(m, "second");
	vector_remove(m, 1);
	vector_set(m, 0, "I'd");
	vector_pop_back(m);
	vector_push_back(m, "moment...");

	vector_t* k = vector_copy(m);

	test_assert(k);
	test_assert(vector_eq(k, m));

	vector_clear(m);

	test_assert(vector_empty(m));

	vector_free(m);

	vector_t* j = vector_init(10, sizeof(char*), NULL);
	vector_push_back(j, "pranked");
	vector_swap(j, k);

	test_seq(vector_att(k, 0, char*), "pranked");

	char* buffer = malloc(sizeof("I'd just like to interject for a moment... ") + 1);
	char* b = buffer;

	for (vector_iterator(j, char*, it)) {
		size_t l = strlen(*it);
		memcpy(b, *it, l);
		b += l;
		*b++ = ' ';
	}

	test_seq(buffer, "I'd just like to interject for a moment... ");
	free(buffer);

	vector_free(j);

	vector_t* heap_vec = vector_init(32, sizeof(vec_example_t*), free);

	vec_example_t* mexa = malloc(sizeof(vec_example_t));
	mexa->mass = 12; mexa->radius = 1; mexa->x = 0; mexa->y = 0;

	vec_example_t* mexb = malloc(sizeof(vec_example_t));
	mexb->mass = 75; mexb->radius = 28; mexb->x = 3; mexa->y = 13;

	vec_example_t* mexc = malloc(sizeof(vec_example_t));
	mexc->mass = 3; mexc->radius = 1; mexc->x = -32; mexc->y = 5;

	vector_push_back(heap_vec, mexa);
	vector_push_back(heap_vec, mexb);
	vector_push_back(heap_vec, mexc);

	test_assert((*(vec_example_t**)vector_at(heap_vec, 1))->mass == 75);

	vector_remove(heap_vec, 1);
	vector_free(heap_vec);
}
