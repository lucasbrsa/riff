#include "hashmap.h"

#include <malloc.h>

/* hash a key (string of characters) using the hashmap size */
unsigned int get_hashcode(char* key);

/* get a bucket index from a hashed key, given the hashmap size */
unsigned int get_index(unsigned hashcode, size_t s);

/* raise a number to a power, useful for the hashing algorithm implemented */
int power(int base, int exponent);

/* insert data into the hashmap */
_Bool insert(hashmap_bucket_t* list, char* key, void* value);

/* search for a key-value pair in the hashmap */
void* search(hashmap_t* map, char* key);

unsigned int get_hashcode(char* key) {
	unsigned int hashcode = 0;
	const char* str = (char*)key;

	/* uses the Java hashing algorithm */
	for (int i = 0, len = strlen(str); i < len; i++) {
		hashcode += (int)str[i] * power(31, len - (i + 1));
	}

	return hashcode;
}

unsigned int get_index(unsigned hashcode, size_t s) {
	return +(hashcode % s); /* std::abs is too kool for me */
}

hashmap_t* hashmap_create(size_t size) {
	hashmap_t* h = malloc(sizeof(hashmap_t));

	if (h) {
		h->buckets = calloc(sizeof(hashmap_bucket_t), size);

		h->size = size;
	}

	return h;
}

void hashmap_free(hashmap_t* map) {

	for (int i = 0; i < map->size; i++)
	{
		hashmap_bucket_t* b = &map->buckets[i];
		while (b->next != NULL) {
			hashmap_bucket_t* t = b;
			b = t->next;
			free(b);
		}
		//free((void*)&map->buckets[i]);
	}

	free(map);
}

_Bool hashmap_set(hashmap_t* map, char* key, void* value) {
	/* calculate the bucket we'll be inserting into */
	hashmap_bucket_t* hb = &map->buckets[get_index(get_hashcode(key), map->size)];

	return insert(hb, key, value);
}

void* hashmap_get(hashmap_t* map, char* key) {
	return search(map, key);
}

int power(int base, int exponent) {
	/* so we don't get caught out!1!! */
	if (!exponent) return 1;

	/* yeah yeah iterate and be cool */
	int result = 1;
	for (int i = 0; i < exponent; i++)
		result *= base;

	return result;
}

_Bool insert(hashmap_bucket_t* head, char* key, void* value) {

	if (!head || !key || !value) { return 0; }

	if (head->next != NULL)
	{

		hashmap_bucket_t* current = head;

		do {
			if (!strcmp(current->key, key)) {
				current->value = value;
				goto ret;
			}
			current = current->next;
		} while (current->next != NULL);

		hashmap_bucket_t* hb = malloc(sizeof(hashmap_bucket_t));

		hb->next = head->next;
		hb->key = key;
		hb->value = value;
		head->next = hb;

	}
	else {
		head->key = key;
		head->value = value;
		head->next = malloc(sizeof(hashmap_bucket_t));
		head->next->key = NULL;
		head->next->value = NULL;
	}

ret:
	return 1;
}

void* search(hashmap_t* map, char* key) {
	/* calculate which bucket is of interest */
	hashmap_bucket_t* current = &(map->buckets[get_index(get_hashcode(key), map->size)]);

	if (!map || !key) { return 0; }

	/* step through, checking if the key matches and return if it does */
	do {
		if (!strcmp(current->key, key))
			return current->value;
		current = current->next;
	} while (current->next != NULL);

	return 0;
}

void* hashmap_resize(hashmap_t* map, size_t new_size)
{
	/* yeah the boys */
	return realloc(map, sizeof(map) * new_size);
}
