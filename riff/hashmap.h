#ifndef _HASHMAP_H
#define _HASHMAP_H

#include <string.h>

typedef struct hashmap_bucket_t {
	char* key;
	void* value;
	struct hashmap_bucket_t* next;
} hashmap_bucket;

typedef struct hashmap_t {
	hashmap_bucket* buckets;
	size_t size;
} hashmap;

hashmap_bucket bucket_create(char* key, void* value, hashmap_bucket* next);
unsigned int get_hashcode(char* key, size_t s);
unsigned int get_index(int hashcode);
int power(int base, int exponent);
_Bool insert(hashmap_bucket* list, char* key, void* value);
void* search(hashmap* map, char* key);


hashmap hashmap_create(size_t size);
void hashmap_free(hashmap* map);

_Bool hashmap_set(hashmap* map, char* key, void* data);
void* hashmap_get(hashmap* map, char* key);

void* hashmap_resize(hashmap* map);

#endif
