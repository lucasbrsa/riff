/* @TODO hashmap_find */

#ifndef _HASHMAP_H
#define _HASHMAP_H

/* number of buckets may not go below this */
#define HASHMAP_MINSIZE (4)

#include "generic.h"

/* if the data in the hashmap is heap allocated, call this before destroying */
typedef void(*hashmap_deleter_t)(void*);

/* bucket is a singly linked-list type */
typedef struct hashmap_bucket_s {
	const char* key;
	void* value;
	struct hashmap_bucket_s* next;
} hashmap_bucket_t;

/* a hashmap is an array of buckets */
typedef struct {
	hashmap_bucket_t* buckets;
	hashmap_deleter_t del;
	size_t size, imems;
} hashmap_t;

/* create and initialise a hashmap with some size, pass NULL to del if none is used */
/* size should be choosen carefully as a hashmap CANNOT be realloced after init */
hashmap_t* hashmap_init(size_t size, hashmap_deleter_t del);

/* free all the memory used by a hashmap */
void hashmap_free(hashmap_t* map);

/* insert/modify data in the hashmap */
bool hashmap_insert(hashmap_t* map, const char* key, void* value);

/* retrieve data from the hashmap given a look-up key, NULL if none exists */
void* hashmap_at(hashmap_t* map, const char* key);

/* get the int hash of a string */
size_t hashmap_hash(hashmap_t* map, const char* key);

/* iterator: front of the map, first value */
hashmap_bucket_t* hashmap_front(hashmap_t* map);

/* how many elements in bucket n */
size_t hashmap_bucket_size(hashmap_t* map, size_t n);

/* get a key based on value */
const char* hashmap_find(hashmap_t* map, void* value);

/* is there an element with that key in the map */
bool hashmap_exists(hashmap_t* map, const char* key);

/* get data from the structure */
#define hashmap_bucket_count(map) ((map)->size)
#define hashmap_size(map) ((map)->imems)

/* is the hashmap empty */
#define hashmap_empty(map) \
	(!(map)->imems)

/* get the nth bucket */
#define hashmap_nbucket(map, n) \
	((map)->buckets + (n * sizeof(hashmap_bucket_t)))

/* get the bucket where a key is located */
#define hashmap_bucket(map, key) \
	hashmap_nbucket(map, hashmap_hash(map, key))

/* load factor is the ratio between bucket count and map size */
#define hashmap_load_factor(map) \
	((float)hashmap_size(map) / (float)hashmap_bucket_count(map))

/* needed functions */
/* back */
/* front */
/* next */
/* erase */
/* clear */
/* swap */
/* reserve */

#endif
