#ifndef _HASHMAP_H
#define _HASHMAP_H
#include "hashmap.h"
#include <malloc.h>

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
	hashmap h;
	h.buckets = calloc(sizeof(bucket), size);
	h.size = size;
	return h;
}

void hashmap_free(hashmap* map) {
	for (int i = 0; i < map->size; i++) 
		free((void*)&map->buckets[i]);

	free(map);
}

_Bool hashmap_set(hashmap* map, char* key, void* value) {
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
	hashmap_bucket hb;
	hb.key = key;
	hb.value = value;
	hb.next = next;
	return b;
}

_Bool insert(hashmap_bucket* head, char* key, void* value) {

	hashmap_bucket hb;
	hb.next = head.next;
	head.next = &hb;
	hb.key = key;
	hb.value = value;

	return 1;
}

void* search(hashmap* map, char* key) {
	hashmap_bucket* current = &(map->buckets[get_index(get_hashcode(key), map->size)]);

	do {
		if (current->key == key)
			return current->value;
		current = current->next;
	} while (current->next != NULL);

	return NULL;
}

void* hashmap_resize(hashmap* map, size_t new_size)
{
	return realloc(map, sizeof(map) * new_size);
}
#endif
