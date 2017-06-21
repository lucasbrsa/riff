/*
#include "hashmap.h"
#include <stdio.h>
#include <malloc.h>
#include <string.h>

bucket* search_for_key(bucket* root, void* key_to_find);
unsigned int get_hashcode(void* key);
int power(int base, int exponent);
unsigned int get_index(unsigned int hashcode);
bucket* create(hashmap_pair data, bucket* next);
bool add_to_start(bucket* start, hashmap_pair data);
bucket* search_for_key(bucket* root, void* key_to_find);

bool set(hashmap* map, void* key, void* value)
{
	hashmap_pair data = { key, value };
	unsigned hashcode = get_hashcode(key);
	unsigned index = get_index(hashcode);
	printf("Hashcode for %s is %i and the index will be %i\n", (char*)key, hashcode, index);
	
	return add_to_start(&(map->buckets[get_index(get_hashcode(key))]), data);
}

void* get(hashmap* map, void* key)
{
	return search_for_key(&(map->buckets[get_index(get_hashcode(key))]), key)->data.value;
}

unsigned int get_hashcode(void* key)
{
	unsigned int hashcode = 0;
	const char* str = (char*)key;
	for (int i = 0, len = strlen(str); i < len; i++) { hashcode += (int)str[i] * power(31, len - (i + 1)); }
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
	memcpy(&b->data, &data, sizeof(data));
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
		if (cursor->data.key == key_to_find)
			return cursor;
		else cursor = cursor->next;
	}
	return 0;
}
*/

#if 1
#include "hashmap.h"
#include <stdio.h>
#include <malloc.h>
#include <string.h>

bucket create(hashmap_pair data, bucket* next);
int get_hashcode(void* key);
unsigned int get_index(int hashcode);
int power(int base, int exponent);
bool insert(bucket* list, hashmap_pair data);
void* search(hashmap* map, void* key);

unsigned int get_hashcode(void* key)
{
	unsigned int hashcode = 0;
	const char* str = (char*)key;
	for (int i = 0, len = strlen(str); i < len; i++) { hashcode += (int)str[i] * power(31, len - (i + 1)); }
	return hashcode;
}

unsigned int get_index(int hashcode)
{
	return +(hashcode % MAX_MAP_SIZE);
}

hashmap hashmap_create()
{
	hashmap h = *(hashmap*)malloc(sizeof(hashmap));
	hashmap_pair tmp = hashmap_pair_create(0, 0);
	for (int i = 0; i < MAX_MAP_SIZE; i++)
	{
		h.buckets[i] = create(tmp, 0);
	}
	return h;
}

void hashmap_free(hashmap* map)
{
	for (int i = 0; i < MAX_MAP_SIZE; i++) { free((void*)&map->buckets[i]); }
	free(map);
}

hashmap_pair hashmap_pair_create(void* key, void* value)
{
	hashmap_pair p = *(hashmap_pair*)malloc(sizeof(key) * sizeof(value));
	p.key = key; p.value = value;
	return p;
}

void hashmap_pair_free(hashmap_pair* pair)
{
	free(pair->key);
	free(pair->value);
	free(pair);
}

bool set(hashmap* map, hashmap_pair data)
{
	printf("%s", map->buckets[0].data.key);
	return insert(&map->buckets[get_index(get_hashcode(data.key))], data);
}

void* get(hashmap* map, void* key)
{
	return search(&map->buckets[get_index(get_hashcode(key))], key);
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

bucket create(hashmap_pair data, bucket* next)
{
	bucket* b = malloc(sizeof(bucket));
	b->data = hashmap_pair_create(data.key, data.value);
	b->next = next;
	return *b;
}

bool insert(bucket* list, hashmap_pair data)
{
	/*list->next = malloc(sizeof(bucket*));
	*(list->next) = create(data, 0);
	bucket* tmp = list->next;
	*(list->next) = create(data, tmp);
	return true;*/
	bucket * current = list;
	while (current->next != NULL) {
		current = current->next;
	}

	/* now we can add a new variable */
	//current = malloc(sizeof(bucket));
	current->data = data;
	//current->next->next = NULL;
	current->next = NULL;

	return true;
}

void* search(hashmap* map, void* key)
{
	bucket* cursor = malloc(sizeof(bucket*));
	memcpy(cursor, (void*)&map->buckets[get_index(get_hashcode(key))], sizeof(bucket));
	//map->buckets[get_index(get_hashcode(key))];
	while(cursor->next != 0)
	{
		//if (memcmp((void*)&cursor->data, (void*)&pair_to_find, sizeof(pair_to_find))) { return cursor; }
		if (memcmp(cursor->data.key, key, sizeof(key))) { return cursor->data.key; }
		else { cursor = cursor->next; }
	}
	return 0;
}

#endif