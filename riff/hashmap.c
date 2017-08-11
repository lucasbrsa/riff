#if 1
#include "hashmap.h"

#include <malloc.h>

hashmap_bucket bucket_create(char* key, void* value, hashmap_bucket* next);
unsigned int get_hashcode(char* key);
unsigned int get_index(int hashcode);
int power(int base, int exponent);
_Bool insert(hashmap_bucket* list, char* key, void* value);
void* search(hashmap* map, char* key);

unsigned int get_hashcode(char* key) {
	unsigned int hashcode = 0;
	const char* str = (char*)key;
	
	for (int i = 0, len = strlen(str); i < len; i++) { 
		hashcode += (int)str[i] * power(31, len - (i + 1)); 
	}
	
	return hashcode;
}

unsigned int get_index(unsigned hashcode, size_t s) {
	return +(hashcode % s);
}

hashmap hashmap_create(size_t size) {
	hashmap h = *(hashmap*)malloc(sizeof(hashmap));
	for (int i = 0; i < size; i++)
		h.buckets[i] = bucket_create(tmp, 0);

	return h;
}

void hashmap_free(hashmap* map) {
	for (int i = 0; i < map->size; i++) 
		free((void*)&map->buckets[i]);

	free(map);
}

_Bool hashmap_set(hashmap* map, char* key, void* value) {
	map->buckets[get_index(get_hashcode(key), map->size)].value = "meme";
	return insert(&map->buckets[get_index(get_hashcode(key), map->size)], key, value);
}

void* hashmap_get(hashmap* map, char* key) {
	return search(map, key);
}

int power(int base, int exponent) {
	if (!exponent) return 1;

	int result = 1;
	for (int i = 0; i < exponent; i++)
		result *= base;
	
	return result;
}

hashmap_bucket bucket_create(char* key, void* value, hashmap_bucket* next) {
	hashmap_bucket* b = malloc(sizeof(hashmap_bucket));
	//b->data = hashmap_pair_create(data.key, data.value);
	b->next = next;
	return *b;
}

_Bool insert(hashmap_bucket* list, char* key, void* value) {
		
	hashmap_bucket* current = list;
	
	while (current->next != NULL) 
		current = current->next;
	current->key = key;
	current->value = value;
	current->next = NULL;
	
	return 1;
}

void* search(hashmap* map, char* key) {
	hashmap_bucket* current = &(map->buckets[get_index(get_hashcode(key), map->size)]);

	do {
		if (current->key == key)
			return current->value;
		current = current->next;
	} while (current->next != NULL);

	return 0;
	//return (*map).buckets[get_index(get_hashcode(key))].data.value;
}

void* hashmap_resize(hashmap* map, size_t new_size)
{
	return realloc(map, sizeof(map) * new_size);
}
#endif