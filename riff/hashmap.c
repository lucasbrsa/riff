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
		h->buckets = calloc(h->size, sizeof(hashmap_bucket_t));
	}

	return h;
}

size_t hashmap_hash(hashmap_t* map, const char* key) {
	size_t hashcode = 0, len = (key)?strlen(key):0;

	/* java hashing algorithm */
	for (signed i = 0; i < len; i++)
		hashcode += (size_t)(key[i] * pow(31.0, (double)(len - (i + 1))));

	return hashcode % map->size;
}

void hashmap_free(hashmap_t* map) {
	hashmap_clear(map);
	free(map);
}

void hashmap_iterate(hashmap_t* map, void(*func)(hashmap_bucket_t*, void*), void* extra) {
	if (hashmap_empty(map)) return;

	for (size_t i = 0; i < map->size; i++) {
		hashmap_bucket_t* b = hashmap_nbucket(map, i);

		if (b->key)
			for (; b; b = b->next)
				func(b, extra);
	}
}

bool hashmap_set(hashmap_t* map, const char* key, void* value) {
	hashmap_bucket_t *head = hashmap_bucket(map, key), *it;

	if (!key)
		return false;

	if (head->key)
	{
		for (it = head; it; it = it->next) {
			if (it->key && strcmp(it->key, key) == 0) {
				head->value = value;
				return true;
			}
		}

		it = malloc(sizeof(hashmap_bucket_t));
		it->key = key;
		it->value = value;

		it->next = head->next;
		head->next = it;

	}
	else {
		head->key = key;
		head->value = value;
	}

	map->imems++;

	return true;
}

void* hashmap_get(hashmap_t* map, const char* key) {
	hashmap_bucket_t* current = hashmap_bucket(map, key);

	if (!key || !current->key)
		return NULL;

	while (current) {
		if (current->key && strcmp(current->key, key) == 0)
			return current->value;

		current = current->next;
	}

	return NULL;
}

bool hashmap_remove(hashmap_t* map, const char* key) {
	hashmap_bucket_t* target = hashmap_bucket(map, key);

	if (!key || !target->key)
		return false;

	map->imems--;

	while (target) {
		if (target->key && strcmp(target->key, key) == 0) {
			if (map->del)
				map->del(target->value);

			if (!target->next)
				memset(target, 0, sizeof(hashmap_bucket_t));
			else {
				hashmap_bucket_t* btmp = target->next;
				memcpy(target, target->next, sizeof(hashmap_bucket_t));
				free(btmp);
			}

			return true;
		}

		target = target->next;
	}

	return false;
}

void hashmap_clear(hashmap_t* map) {
	for (size_t i = 0; i < map->size; i++) {
		hashmap_bucket_t *b = hashmap_nbucket(map, i), *t;

		if (b->key && map->del && b->value)
			free(b->value);

		b = b->next;
		while (b) {
			t = b;
			b = b->next;

			if (map->del)
				free(t->value);

			free(t);
		}
	}

	memset(map->buckets, 0, sizeof(hashmap_bucket_t) * map->size);
	map->imems = 0;
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

const char* hashmap_find(hashmap_t* map, void* value) {
	for (size_t it = 0; it < map->size; it++) {
		hashmap_bucket_t* b = hashmap_nbucket(map, it);
		while (b->next) {
			if (b->value == value)
				return b->key;

			b = b->next;
		}
	}

	return NULL;
}
