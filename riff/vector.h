#ifndef _VECTOR_H
#define _VECTOR_H

#include "generic.h"

typedef struct {
	char* data;
	size_t capacity, size, blksz;
} vector_t;

/* create a vector object */
vector_t* vector_init(size_t start_len, size_t element_size);

/* copy a vector object */
vector_t* vector_copy(const vector_t* v);

/* free the memory allocated by the vector object */
void vector_free(vector_t* v);

/* realloc the vector object */
_Bool vector_realloc(vector_t* v, size_t len);

/* reserve extra space for the vector object */
_Bool vector_reserve(vector_t* v, size_t len);

/* append element to the end of the vector object */
_Bool vector_push_back(vector_t* v, const void* val);

/* append elements to the end of the vector object */
_Bool vector_append(vector_t* v, const void* vals, size_t val_count);

/* remove element from the end of the vector object */
void vector_pop_back(vector_t* v);

/* delete all elements from the vector object  */
void vector_clear(vector_t* v);

/* insert element into the vector object */
_Bool vector_insert(vector_t* v, size_t index, void* val);

/* remove element from the vector object */
void vector_remove(vector_t* v, size_t index);

/* get the first element in the vector object, for iteration */
void* vector_begin(const vector_t* v);

/* get the last element in the vector object, for iteration */
void* vector_end(const vector_t* v);

/* get the next element in the vector object, for iteration */
void* vector_next(const vector_t* v, void* i);

/* get the data stored at index in the vector object */
void* vector_at(const vector_t* v, size_t index);

/* change the value stored at index */
void* vector_set(vector_t* v, size_t index, void* val);

/* convert to string, an array of integers */
char* vector_joini(char* dest, vector_t* v, char* delimiter);

/* convenient function */
void vector_printi(vector_t* v);

#endif