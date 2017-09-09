/* includes */
#include "hashmap.h"

#include <malloc.h>

/* hash a key (string of characters) using the hashmap size */
unsigned int get_hashcode(char* key);

/* get a bucket index from a hashed key, given the hashmap size */
unsigned int get_index(unsigned hashcode, size_t s);

/* raise a number to a power, useful for the hashing algorithm implemented */
int power(int base, int exponent);

/* insert data into the hashmap */
_Bool insert(hashmap_bucket* list, char* key, void* value);

/* search for a key-value pair in the hashmap */
void* search(hashmap* map, char* key);

/* hashes a key */
unsigned int get_hashcode(char* key) {
	
	/* shitty implementation */
	unsigned int hashcode = 0;

	/* why */
	const char* str = (char*)key;

	/* uses the Java hashing algorithm */
	for (int i = 0, len = strlen(str); i < len; i++) {
		hashcode += (int)str[i] * power(31, len - (i + 1));
	}

	return hashcode;
}

/* get the bucket index from the hashmap */
unsigned int get_index(unsigned hashcode, size_t s) {
	return +(hashcode % s); /* std::abs is too kool for me */
}

/* create a hashmap, anon's favourite function! */
hashmap hashmap_create(size_t size) {
	
	hashmap h;

	/* callocate memory for the buckets */ /* i'm funny */
	h.buckets = calloc(sizeof(hashmap_bucket), size);
	
	/* set size */
	h.size = size;
	
	/* ret */
	return h;
}

/* free the mem used by the lame hash map */
void hashmap_free(hashmap* map) {

	/* iterate and call free() on all the memes */
	for (int i = 0; i < map->size; i++)
		free((void*)&map->buckets[i]);

	free(map);
}

/* set a value in the hashmap */
_Bool hashmap_set(hashmap* map, char* key, void* value) {
	
	/* calculate the bucket we'll be inserting into */
	hashmap_bucket* hb = &map->buckets[get_index(get_hashcode(key), map->size)];
	
	/* insert */
	return insert(hb, key, value);
}

/* get a value associated to a key from a hashmap */
void* hashmap_get(hashmap* map, char* key) {
	/* call the linked-list searcher */
	return search(map, key);
}

/* raise a number, lame meme */
int power(int base, int exponent) {
	
	/* so we don't get caught out!1!! */
	if (!exponent) return 1;

	/* yeah yeah iterate and be cool */
	int result = 1;
	for (int i = 0; i < exponent; i++)
		result *= base;

	return result;
}

/* insert into a linked list */
_Bool insert(hashmap_bucket* head, char* key, void* value) {

	/* check if the linked list is not sized 1 */
	if (head->next != NULL)
	{
		/* bucket to add */
		hashmap_bucket* hb = malloc(sizeof(hashmap_bucket));
		
		/* set properties */
		hb->next = head->next;
		hb->key = key;
		hb->value = value;
		head->next = hb;

	} else {
		/* inserting into the first element */
		head->key = key;
		head->value = value;
	}
	
	return 1;
}

/* search in a linked list */
void* search(hashmap* map, char* key) {
	/* calculate which bucket is of interest */
	hashmap_bucket* current = &(map->buckets[get_index(get_hashcode(key), map->size)]);

	/* step through, checking if the key matches and return if it does */
	do { /* at least once */
		if (current->key == key)
			return current->value;
		current = current->next;
	} while (current->next != NULL);

	return NULL;
}

/* resize the hashmap */
void* hashmap_resize(hashmap* map, size_t new_size)
{
	/* reallocate memory */
	return realloc(map, sizeof(map) * new_size);
}
