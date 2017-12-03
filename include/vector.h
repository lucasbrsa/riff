/* simple vector, dynamically sized array */
/* stores an array of pointers */
/* can delete the deref of those */
/* untested, like most code.. */

#ifndef _VECTOR_H
#define _VECTOR_H

#include <stdio.h>

#include "generic.h"

/* the minimum size allowed for a vector */
#define VECTOR_MINSIZE 1

/* defines the eqn of growth */
#define VECTOR_GROWTH(n) (2*((n)+1))

/* a function call when members of a vector are heap allocated */
typedef void(vector_del_f)(void* ptr);

/* the vector */
typedef struct {
	char* data;
	size_t capacity, size, blksz;
	vector_del_f* deleter;
} vector_t;

/* create a vector, initializing with start_len and the sizeof esach element */
/* deleter defines the function to call on deletion or NULL */
vector_t* vector_init(size_t start_len, size_t element_size, vector_del_f* deleter);

/* create a new vector, copying another into it */
vector_t* vector_copy(const vector_t* v);

/* free the memory allocated by the vector */
void vector_free(vector_t* v);

/* realloc the vector */
bool vector_realloc(vector_t* v, size_t len);

/* reserve extra space for the vector */
bool vector_reserve(vector_t* v, size_t len);

/* append multiple elements to the end of the vector */
bool vector_append(vector_t* v, const void* vals, size_t val_count);

/* add one element to the end of the vector */
bool vector_push_back(vector_t* v, void* val);

/* delete all elements of the vector */
void vector_clear(vector_t* v);

/* insert element into the vector */
bool vector_insert(vector_t* v, size_t index, void* val);

/* remove element from the vector */
bool vector_remove(vector_t* v, size_t index);

/* change the value stored at index */
void* vector_set(vector_t* v, size_t index, void* val);

/* swap each element of v1 with the elements of v2 and vice versa */
/* may be used to resize a vector down */
void vector_swap(vector_t* v1, vector_t* v2);

/* remove i element from the tail of a vector */
void vector_pop(vector_t* v, size_t i);

/* compare if two vectors are equal (byte wise comparison) */
/* sign of ret cannot be trusted, is NOT lexicographic compare */
bool vector_eq(vector_t* v1, vector_t* v2);

/* remove one element from the tail of a vector */
#define vector_pop_back(v) \
	vector_pop(v, 1)

/* resize a vector, will never decrease capacity */
#define vector_resize(v, s) \
	vector_reserve(v, s)

/* get the data stored at index in the vector, will return a pointer to it */
#define vector_at(v, i) \
	(void*)((v)->data + ((i) * (v)->blksz))

/* get, and dereference, the data stored at index in the vector */
#define vector_att(v, i, type) \
	(*(type*)((v)->data + ((i) * (v)->blksz)))

/* iterators, done via pointer comparison */

/* get the first element in the vector */
#define vector_front(v) \
	((void*)(v)->data)

/* get the next element given an iterator */
#define vector_next(v, it) \
	((void*)((char*)it + (v)->blksz))

/* get the last element in the vector */
#define vector_back(v) \
	(void*)((v)->data + ((v)->size * (v)->blksz))

/* like a C++11 iterator, put in a for loop */
#define vector_iterator(v, type, i) \
	type* i = vector_front(v); i != vector_back(v); i = vector_next(v, i)

/* is the vector empty? */
#define vector_empty(v) \
	(!(v)->size)

/* more intuitive init function */
#define vector_construct(type) vector_init(0, sizeof(type), NULL)

/* poll data from the structure */
#define vector_size(v) ((v)->size)
#define vector_data(v) ((v)->data)
#define vector_blksze(v) ((v)->blksze)
#define vector_capacity(v) ((v)->capacity)

/* mainly for debugging purposes */
#define vector_print(v, type, fmt_mod) \
	do { void *beg = vector_first((v)), *end = vector_back((v)); \
		for (void* it = beg; it != end; it = vector_next(v, it)) \
			{ printf((fmt_mod), *(type*)it); } \
	} while(0)

#endif
