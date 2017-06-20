#include "hashmap.h"
#include <stdbool.h>
#include <malloc.h>
#include <string.h>

typedef struct hashmap_pair_t
{
	void* key;
	void* value;
} hashmap_pair;

typedef struct bucket_t
{
	hashmap_pair data;
	void* data;
	struct bucket_t* next;
} bucket;

typedef struct hashmap_t
{
	bucket* buckets[MAX_MAP_SIZE];
} hashmap;

/*
hash_map hashmap_new() 
{
	hashmap* m = (hashmap*)malloc(sizeof(hashmap));
	if (!m) goto err;
err:
	if (m)
		hashmap_free(m);
	return 0;
}
*/

bucket* search_for_key(bucket* root, void* key_to_find);
unsigned int get_hashcode(void* key);
int power(int base, int exponent);
unsigned int get_index(unsigned int hashcode);
bucket* create(hashmap_pair data, bucket* next);
bool add_to_start(bucket* start, hashmap_pair data);
bucket* search_for_key(bucket* root, void* key_to_find);

bool set(hash_map* map, void* key, void* value)
{

	hashmap_pair data = { key, value };
	return add_to_start(((hashmap*)map)->buckets[get_index(get_hashcode(key))], data);
}

void* get(hash_map* map, void* key)
{
	return search_for_key((*(hashmap*)&map).buckets[get_index(get_hashcode(key))], key)->data.value;
}

unsigned int get_hashcode(void* key)
{
	unsigned int hashcode = 0;
	for (int i = 0, len = strlen(key); i < len; i++) { hashcode += *(char*)key * power(31, len - (i + 1)); }
	return hashcode;
}

int power(int base, int exponent)
{
	if (!exponent)
		return 1;
	int result = 1;
	for (int i = 0; i < exponent; i++)
		result *= base;
	return result;
}

unsigned int get_index(unsigned int hashcode)
{
	return hashcode % MAX_MAP_SIZE;
}

bucket* create(hashmap_pair data, bucket* next)
{
	bucket* b = malloc(sizeof(bucket));
	b->data = data;
	b->next = next;
	return b;
}


bool add_to_start(bucket* start, hashmap_pair data)
{
	bucket* to_add = create(data, start);
	start = to_add;
	return true;
}

bucket* search_for_key(bucket* root, void* key_to_find)
{
	bucket* cursor = root;
	while (cursor) {
		if (cursor->data.value == key_to_find)
			return cursor;
		else cursor = cursor->next;
	}
	return 0;
}