#include "vector.h"
#include "generic.h"

#include <malloc.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

_Bool vector_realloc(vector_t* v, size_t len) {
	char* r = realloc(v->data, len * v->blksz); /* will behave as malloc when v is 0 */

	if (!r) 
		return 0;

	v->data = r;
	v->capacity = len;

	return 1;
}

_Bool vector_reserve(vector_t* v, size_t len) {
	if (len < v->capacity)
		return 0;

	return vector_realloc(v, len);
}

vector_t* vector_init(size_t start_len, size_t element_size) {
	vector_t* v = malloc(sizeof(vector_t));

	if (v) {
		start_len = generic_max(start_len, 4);

		v->data = v->size = 0;
		v->blksz = element_size;

		if (!element_size || !vector_realloc(v, start_len))
			v = 0, free(v);
	}
	
	return v;
}

vector_t* vector_copy(const vector_t* v) {
	vector_t* k = vector_init(v->capacity, v->blksz);
	if (!memcpy(k->data, v->data, v->size * v->blksz)) {
		vector_free(k);
		return 0;
	}

	k->size = v->size;
	return k;
}

void vector_free(vector_t* v) {
	if (v->capacity)
		free(v->data);

	free(v);
}

void* vector_at(vector_t* v, size_t index) {
	return v->data + (index * v->blksz);
}

void* vector_begin(const vector_t* v) {
	return v->data;
}

void* vector_end(const vector_t* v) {
	return v->data + (v->size * v->blksz);
}

void* vector_next(const vector_t* v, void* i) {
	return (char*)i + v->blksz;
}

_Bool vector_insert(vector_t* v, size_t index, void* val) {
	if (v->size+1 > v->capacity)
		if (!vector_realloc(v, v->capacity * 2))
			return 0;

	char* o = v->data + (index * v->blksz);
	
	if (!memmove(o + v->blksz, o, v->blksz * (v->size - index)))
		return 0;

	memcpy(o, &val, v->blksz); /* don't feel like checking */
	v->size++;

	return 1;
}

void vector_remove(vector_t* v, size_t index) {
	char* i = v->data + (index * v->blksz);

	if (!memmove(i + v->blksz, i, v->blksz * (v->size - index)))
		return 0;

	v->size--;
	return 1;
}

_Bool vector_append(vector_t* v, const void* vals, size_t val_count) {
	size_t new_count = v->size + val_count;

	if (v->capacity < new_count) {
		size_t amnt = v->capacity;
		
		do amnt *= 2; 
		while (amnt < new_count);
		
		if (!vector_realloc(v, amnt))
			return 0;
	}

	if (!memcpy(v->data + (v->size * v->blksz), &vals, v->blksz * val_count))
		return 0;

	v->size = new_count;

	return 1;
}

_Bool vector_push_back(vector_t* v, const void* val) {
	return vector_append(v, val, 1);
}

void vector_pop_back(vector_t* v) {
	if (!v->size)
		return;

	v->size--;
}

void* vector_set(vector_t* v, size_t index, void* val) {
	return memcpy(v->data + (v->blksz * index), &val, v->blksz);
}

void vector_clear(vector_t* v) {
	v->size = 0;
}

char* vector_joini(char* dest, vector_t* v, char* delimiter) {
	size_t dlen = strlen(delimiter);
	char* o = dest;
	for (int i = 0; i < v->size; i++) {
		dest += sprintf(dest, "%d", *(int*)vector_at(v, i));
		if (i != v->size - 1) {
			memcpy(dest, delimiter, dlen);
			dest += dlen;
		}
	} *dest = 0;

	return o;
}

void vector_printi(vector_t* v) {
	char* buf = malloc(16 * v->capacity);
	vector_joini(buf, v, ", ");
	printf("[%s]\n", buf); free(buf);
}