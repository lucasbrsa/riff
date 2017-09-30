#include "hashmap.h"

#include <math.h>
#include <malloc.h>
#include <string.h>

#define GET_INDEX(h, s) \
	(+((h) % (s)))

unsigned int hashmap_hash(const char* key) {
	unsigned int hashcode = 0, len = (key)?strlen(key):0;

	for (int i = 0; i < len; i++) {
		/* uses the Java hashing algorithm */
		hashcode += (int)key[i] * (int)pow(31.0, (double)(len - (i + 1)));
	}

	return hashcode;
}

hashmap_t* hashmap_init(size_t size) {
	hashmap_t* h = malloc(sizeof(hashmap_t));

	if (h) {
		h->size = (size > HASHMAP_MINSIZE)? size : HASHMAP_MINSIZE;
		h->buckets = calloc(sizeof(hashmap_bucket_t), h->size);
	}

	return h;
}

void hashmap_free(hashmap_t* map) {
	for (int i = 0; i < map->size; i++)
	{
		/* only free individual members if they are NOT the head of the ll */
		hashmap_bucket_t* b = map->buckets[i].next;
		while (b && b->next) {
			hashmap_bucket_t* t = b;
			b = b->next;
			free(t);
		}
	}

	free(map);
}

bool hashmap_set(hashmap_t* map, const char* key, void* value) {
	hashmap_bucket_t* head = &map->buckets[GET_INDEX(hashmap_hash(key), map->size)];

	// if (!head || !key || !value) { return 0; }

	if (!head || !key)
		return 0;

	if (head->next != NULL)
	{
		hashmap_bucket_t* current = head;

		do {
			if (!strcmp(current->key, key)) {
				current->value = value;
				return 1;
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

	return 1;
}

void* hashmap_get(hashmap_t* map, const char* key) {
	/* calculate which bucket is of interest */
	hashmap_bucket_t* current = &(map->buckets[GET_INDEX(hashmap_hash(key), map->size)]);

	if (!map || !key || !current->next)
		return 0;

	/* step through, checking if the key matches and return if it does */
	do {
		if (current->key && !strcmp(current->key, key))
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
