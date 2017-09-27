#ifndef _HASHMAP_H
#define _HASHMAP_H

#include "generic.h"

#include <string.h>

/* hashmap bucket linked-list type */
typedef struct hashmap_bucket_s {
	const char* key; /* hashable key */
	void* value; /* stored value */
	struct hashmap_bucket_s* next; /* points to next element */

} hashmap_bucket_t;

/* defines the internal representation of a hashmap */
typedef struct {
	hashmap_bucket_t* buckets; /* holds a variably sized list of buckets */
	size_t size; /* number of buckets */
} hashmap_t;

/* create and initialise a hashmap with some size */
hashmap_t* hashmap_create(size_t size);

/* free all the memory used by a hashmap */
void hashmap_free(hashmap_t* map);

/* insert data into the hashmap with a hashable key */
bool hashmap_set(hashmap_t* map, const char* key, void* data);

/* retrieve data from the hashmap given a look-up key */
void* hashmap_get(hashmap_t* map, const char* key);

/* resize the number of buckets to a given new size */
void* hashmap_resize(hashmap_t* map, size_t new_size);

/* get the data stored at index */
#define hashmap_at(map, idx) \
	((map)->buckets + ((idx) * sizeof(hashmap_bucket_t)))

/* get the first element, for iteration */
#define hashmap_begin(map) \
	((map)->buckets)

/* get the last element in the, for iteration */
#define hashmap_end(map) \
	((map)->buckets + ((map)->size * sizeof(hashmap_bucket_t)))

/* get the next bucket, for iteration */
#define hashmap_next(it) \
	((it) += sizeof(hashmap_bucket_t))

/* poll the length of the map */
#define hashmap_size(map) \
	((map)->size)

#endif
