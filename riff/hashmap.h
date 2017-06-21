/*
#ifndef _HASHMAP_H
#define _HASHMAP_H

#include <stdbool.h>

#define MAX_MAP_SIZE 10

typedef struct hashmap_pair_t
{
	void* key;
	void* value;
} hashmap_pair;

typedef struct bucket_t
{
	hashmap_pair data;
	struct bucket_t* next;
} bucket;

typedef struct hashmap_t
{
	bucket buckets[MAX_MAP_SIZE];
} hashmap;

bool set(hashmap* map, void* key, void* value);
void* get(hashmap* map, void* key);
unsigned int get_hashcode(void* key);
#endif
*/

#if 1
#ifndef _HASHMAP_H
#define _HASHMAP_H

#include <stdbool.h>

#define MAX_MAP_SIZE 10

//alloc hashmap_pair given key and value size in bytes (sizeof())
typedef struct hashmap_pair_t
{
	void* key;
	void* value;
} hashmap_pair;

typedef struct bucket_t
{
	hashmap_pair data; // need to alloc this data before use
	struct bucket_t* next;
} bucket;

typedef struct hashmap_t
{
	bucket buckets[MAX_MAP_SIZE];
} hashmap;

hashmap hashmap_create();
void hashmap_free(hashmap* map);

hashmap_pair hashmap_pair_create(void* key, void* value);
void hashmap_pair_free(hashmap_pair* pair);

bool set(hashmap* map, hashmap_pair data); 
void* get(hashmap* map, void* key);

#endif
#endif