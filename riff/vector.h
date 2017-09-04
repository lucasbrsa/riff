/* simple vector, dynamically sized array */
/* stores an array of pointers */
/* can delete the deref of those */
/* untested, like most code.. */

#ifndef _VECTOR_H
#define _VECTOR_H

#include "generic.h"

/* the minimum size allowed for a vector */
#define VECTOR_MINSIZE 4

/* defines the eqn of growth */
#define VECTOR_GROWTH(n) (2*(n))

/* defines the eqn of shrinkage */
/* vector shrinkage not yet implemented */
#define VECTOR_SHRINK(n) ((n)/2)

/* a function call when memebers of a vecotr are heap allocated */
typedef void(*vector_del_f)(void* ptr);

/* the vector */
typedef struct {
	char* data;
	size_t capacity, size, blksz;
	vector_del_f* deleter;
} vector_t;

/* create a vector, initializing with start_len; deleter defines the function to call on deletion or NULL */
vector_t* vector_init(size_t start_len, size_t element_size, vector_del_f* deleter);

/* create a new vector, copying another into it */
vector_t* vector_copy(const vector_t* v);

/* free the memory allocated by the vector */
void vector_free(vector_t* v);

/* realloc the vector */
_Bool vector_realloc(vector_t* v, size_t len);

/* reserve extra space for the vector */
_Bool vector_reserve(vector_t* v, size_t len);

/* append multple elements to the end of the vector */
_Bool vector_append(vector_t* v, const void* vals, size_t val_count);

/* delete all elements of the vector */
void vector_clear(vector_t* v);

/* insert element into the vector */
_Bool vector_insert(vector_t* v, size_t index, void* val);

/* remove element from the vector */
void vector_remove(vector_t* v, size_t index);

/* change the value stored at index */
void* vector_set(vector_t* v, size_t index, void* val);

/* print all values in v, assuming it is storing ints */
void vector_printi(vector_t* v);

/* get the data stored at index in the vector */
#define vector_at(v, i) \
	(void*)((v)->data + ((i) * (v)->blksz))

/* get the first element in the vector, for iteration */
#define vector_begin(v) \
	((void*)(v)->data)

/* get the last element in the vector, for iteration */
#define vector_end(v) \
	(void*)((v)->data + ((v)->size * (v)->blksz))

/* get the next element in the vector, for iteration */
#define vector_next(v, e) \
	(void*)((char*)(e) + (v)->blksz;)

/* remove element from the end of the vector */
#define vector_push_back(v, e) \
	(_Bool)(vector_append((v), (e), 1))

#endif