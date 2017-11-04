#include "test.h"
#include "hashmap.h"
#include "str.h"

void iscool(hashmap_bucket_t* b, void* v) {
	if (strcmp(b->key, "cameron") == 0)
		str_cpy(v, b->value);
}

void hashmap_test(void) {
	/* realistically a table of length 2 is absurd, but this is to maximize collisions */
	hashmap_t* hm = hashmap_init(2, NULL);

	test_assert(hashmap_empty(hm));
	hashmap_set(hm, "james", "is a sick cunt");
	hashmap_set(hm, "john", "is just a cunt");
	hashmap_set(hm, "james", "is a great bloke");
	hashmap_remove(hm, "john");
	test_assert(hashmap_length(hm) == 1);

	test_seq(hashmap_get(hm, "james"), "is a great bloke");
	test_assert(hashmap_get(hm, "peter") == NULL);

	hashmap_set(hm, "jim", "is lame");
	hashmap_set(hm, "jerry", "is stupid");
	hashmap_set(hm, "rick", "is smart");
	hashmap_set(hm, "beth", "is annoying");
	hashmap_set(hm, "summer", "is eh");
	hashmap_set(hm, "morty", "is named after an old jewish comedian");
	hashmap_set(hm, "cameron", "is a god");
	hashmap_set(hm, "ruchir", "is a demigod");

	char* buffer = malloc(128);
	hashmap_iterate(hm, iscool, buffer);
	test_seq(buffer, "is a god");

	hashmap_set(hm, "cameron", "is an idiot");
	hashmap_set(hm, "ruchir", "is a god");

	hashmap_remove(hm, "james");
	hashmap_remove(hm, "jim");
	hashmap_remove(hm, "jerry");
	hashmap_remove(hm, "rick");
	hashmap_remove(hm, "beth");
	hashmap_remove(hm, "summer");
	hashmap_remove(hm, "morty");

	test_assert(hashmap_get(hm, "rick") == NULL);
	test_assert(hashmap_get(hm, "beth") == NULL);

	hashmap_free(hm);

	hashmap_t* memhm = hashmap_init(8, free);

	int* ayy = malloc(sizeof(int));
	int* bae = malloc(sizeof(int));

	*ayy = 69;
	*bae = 420;

	hashmap_set(memhm, "ayy", ayy);
	hashmap_set(memhm, "bae", bae);

	*bae = 720;

	test_assert(*(int*)hashmap_get(memhm, "bae") == 720);

	hashmap_free(memhm);
}

int main() { test_run(hashmap_test); test_resolve(); }
