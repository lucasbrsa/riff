#include "vector.h"
#include "hashmap.h"
#include "log.h"

#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define tassert(m) \
	do { if (!(m)) return #m; } while(0)

#define test(func) \
	do { printf("running test on %s\t", #func); const char* k = func(); \
	if (k) printf("failed: %s\n", k); else printf("success\n"); } while(0)

const char* vector_test(void);
const char* hashmap_test(void);
const char* log_test(void);

int main() {
	test(vector_test);
	test(hashmap_test);
	test(log_test);

	return 0;
}

const char* vector_test(void) {
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

	tassert(k);
	tassert(vector_eq(k, m) == 0);

	vector_clear(m);

	tassert(vector_empty(m));

	vector_free(m);

	vector_t* j = vector_init(10, sizeof(char*), NULL);
	vector_push_back(j, "pranked");
	vector_swap(j, k);

	tassert(strcmp(vector_att(k, 0, char*), "pranked") == 0);

	char* buffer = malloc(sizeof("I'd just like to interject for a moment... ") + 1);
	char* b = buffer;

	for (char** it = vector_front(j); it != vector_back(j); it = vector_next(j, it)) {
		size_t l = strlen(*it);
		memcpy(b, *it, l);
		b += l;
		*b++ = ' ';
	}

	tassert(strcmp(buffer, "I'd just like to interject for a moment... ") == 0);
	free(buffer);

	vector_free(j);

	return NULL;
}

const char* hashmap_test(void) {
	hashmap_t* hm = hashmap_init(16);
	hashmap_set(hm, "james", "is a sick cunt");
	hashmap_set(hm, "john", "is just a cunt");

	tassert(strcmp(hashmap_get(hm, "james"), "is a sick cunt") == 0);
	tassert(hashmap_get(hm, "peter") == NULL);

	hashmap_free(hm);

	return NULL;
}

const char* log_test(void) {
	log_logger_t* my_logger = log_logger_init("test logger", NULL);

	tassert(my_logger != NULL);
	tassert(my_logger == log_logger_get("test logger"));

	log_logger_free(my_logger);
	// log_free(); <-- not working

	return NULL;
}
