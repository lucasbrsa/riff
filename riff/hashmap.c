#include "hashmap.h"

#include <math.h>
#include <malloc.h>
#include <string.h>
#include <assert.h>

hashmap_t* hashmap_init(size_t size, hashmap_deleter_t del) {
	hashmap_t* h = malloc(sizeof(hashmap_t));

	if (h) {
		h->del = del;
		h->imems = 0;
		h->size = MAX(size, HASHMAP_MINSIZE);
		h->buckets = calloc(sizeof(hashmap_bucket_t), h->size);
	}

	return h;
}

size_t hashmap_hash(hashmap_t* map, const char* key) {
	size_t hashcode = 0, len = (key)?strlen(key):0;

	/* java hashing algorithm */
	for (int i = 0; i < len; i++) {
		hashcode += (size_t)key[i] * (size_t)pow(31.0, (double)(len - (i + 1)));
	}

	return +(hashcode % h->size);
}

void hashmap_free(hashmap_t* map) {
	if (hashmap_empty(map)) {
		free(map);
		return;
	}

	for (int i = 0; i < map->size; i++) {
		hashmap_bucket_t* b = map->buckets[i].next;

		if (map->del)
			free(map->buckets[i].value);

		/* only free individual members if they are NOT the head of the ll */
		while (b && b->next) {
			hashmap_bucket_t* t = b;
			b = b->next;

			if (map->del)
				free(t->value);

			free(t);
		}
	}

	free(map);
}

bool hashmap_insert(hashmap_t* map, const char* key, void* value) {
	hashmap_bucket_t* head = hashmap_bucket(map, key);

	if (!head || !key)
		return false;

	if (head->next)
	{
		hashmap_bucket_t* current = head;

		do {
			if (!strcmp(current->key, key)) {
				current->value = value;
				return true;
			}
			current = current->next;
		} while (current->next);

		hashmap_bucket_t* hb = malloc(sizeof(hashmap_bucket_t));

		hb->next = head->next;
		hb->key = key;
		hb->value = value;
		head->next = hb;

	}
	else {
		head->key = key;
		head->value = value;
		head->next = calloc(sizeof(hashmap_bucket_t), 1);
	}

	map->imems++;
	return true;
}

void* hashmap_at(hashmap_t* map, const char* key) {
	/* calculate which bucket is of interest */
	hashmap_bucket_t* current = hashmap_bucket(map, key);

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

size_t hashmap_bucket_size(hashmap_t* map, size_t n) {
	if (n >= map->size)
		return 0;

	size_t r = 0;
	hashmap_bucket_t* b = hashmap_nbucket(map, n);

	while (b->next) {
		r++;
		b = b->next;
	}

	return r;
}

bool hashmap_exists(hashmap_t* map, const char* key);
