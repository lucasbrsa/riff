#if 1
#ifndef _HASHMAP_H
#define _HASHMAP_H

#include <string.h>

/* hashmap linked-list bucket type */
typedef struct hashmap_bucket_t {
	char* key;
	void* value;
	struct hashmap_bucket_t* next;
} hashmap_bucket;

typedef struct hashmap_t {
	hashmap_bucket* buckets;
	size_t size;
} hashmap;

hashmap hashmap_create(size_t size);
void hashmap_free(hashmap* map);

_Bool hashmap_set(hashmap* map, char* key, void* data);
void* hashmap_get(hashmap* map, char* key);

void* hashmap_resize(hashmap* map);

#endif
#endif