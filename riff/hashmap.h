/* @TODO add hashmap deleters */

#ifndef _HASHMAP_H
#define _HASHMAP_H

#include "generic.h"

/* number of buckets may not go below this */
#define HASHMAP_MINSIZE (4)

/* bucket is a singly linked-list type */
typedef struct hashmap_bucket_s {
	const char* key;
	void* value;
	struct hashmap_bucket_s* next;
} hashmap_bucket_t;

/* a hashmap is an array of buckets */
typedef struct {
	hashmap_bucket_t* buckets;
	size_t size;
} hashmap_t;

/* create and initialise a hashmap with some size */
hashmap_t* hashmap_init(size_t size);

/* free all the memory used by a hashmap */
void hashmap_free(hashmap_t* map);

/* insert data into the hashmap with a hashable key */
bool hashmap_set(hashmap_t* map, const char* key, void* data);

/* retrieve data from the hashmap given a look-up key */
void* hashmap_get(hashmap_t* map, const char* key);

/* resize the number of buckets to a given new size */
void* hashmap_resize(hashmap_t* map, size_t new_size);

/* get the int hash of a string */
unsigned hashmap_hash(const char* k);

/* get the data stored at index */
#define hashmap_at(map, idx) \
	((map)->buckets + ((idx) * sizeof(hashmap_bucket_t)))

/* get the first element, for iteration */
#define hashmap_begin(map) \
	((map)->buckets)

/* get the last element in the, for iteration */
#define hashmap_end(map) \
	((map)->buckets + ((map)->size * sizeof(hashmap_bucket_t)))

/* poll the length of the map */
#define hashmap_size(map) \
	((map)->size)

/* needed functions */
/* empty? */
/* size */
/* max_size */
/* back */
/* front */
/* next */
/* at */
/* find */
/* mcount */
/* insert */
/* erase */
/* clear */
/* swap */
/* bucket_count */
/* max_bucket_count */
/* bucket */
/* reserve */

#endif
