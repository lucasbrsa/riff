#ifndef _HASHMAP_H
#define _HASHMAP_H

/* includes */
#include <string.h>

/* hashmap bucket linked-list type */
typedef struct hashmap_bucket_t {
	
	/* hashable key */
	char* key;
	
	/* stored value */
	void* value; 
	
	/* points to next element */
	struct hashmap_bucket_t* next; 

} hashmap_bucket;

/* hashmap type struct */
typedef struct hashmap_t {
	
	/*holds a variably sized list of buckets */
	hashmap_bucket* buckets;

	/* number of buckets */
	size_t size;

} hashmap;

/* create and initialise a hashmap with some size */
hashmap hashmap_create(size_t size);

/* free all the memory used by a hashmap */
void hashmap_free(hashmap* map);

/* insert data into the hashmap with a hashable key */
_Bool hashmap_set(hashmap* map, char* key, void* data);

/* retrieve data from the hashmap given a look-up key */
void* hashmap_get(hashmap* map, char* key);

/* resize the number of buckets to a given new size */
void* hashmap_resize(hashmap* map, size_t new_size);

#endif
