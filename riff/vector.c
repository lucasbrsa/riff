#include "vector.h"
#include "generic.h"

#include <assert.h>
#include <string.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>

void vector_call_deletera(vector_t* v);
void vector_call_deletere(vector_t* v, size_t e);
void vector_call_deleteri(vector_t* v, size_t l, size_t r);

_Bool vector_realloc(vector_t* v, size_t len) {
	char* r = realloc(v->data, len * v->blksz);

	if (!r)
		return 0;

	v->data = r;
	v->capacity = len;

	return 1;
}

void vector_call_deletere(vector_t* v, size_t e) {
	(*v->deleter)(*(void**)(v->data + (e * v->blksz)));
}

void vector_call_deletera(vector_t* v) {
	for (void* b = vector_front(v); b != vector_back(v); b = vector_next(v, b))
		(*v->deleter)(*(void**)b);
}

void vector_call_deleteri(vector_t* v, size_t l, size_t r) {
	for (void* b = vector_at(v, l); b != vector_at(v, r); b = vector_next(v, b))
		(*v->deleter)(*(void**)b);
}

_Bool vector_reserve(vector_t* v, size_t len) {
	if (len < v->capacity)
		return 0;

	return vector_realloc(v, len);
}

vector_t* vector_init(size_t start_len, size_t element_size, vector_del_f* deleter) {
	vector_t* v = malloc(sizeof(vector_t));

	if (v) {
		start_len = MAX(start_len, VECTOR_MINSIZE);

		v->data = NULL; v->size = 0;
		v->blksz = element_size;
		v->deleter = deleter;

		if (!element_size || !vector_realloc(v, start_len))
			v = 0, free(v);
	}

	return v;
}

vector_t* vector_copy(const vector_t* v) {
	vector_t* k = vector_init(v->capacity, v->blksz, v->deleter);

	if (!memcpy(k->data, v->data, v->size * v->blksz)) {
		vector_free(k);
		return 0;
	}

	k->size = v->size;
	return k;
}

void vector_free(vector_t* v) {
	if (v->deleter)
		vector_call_deletera(v);

	if (v->capacity)
		free(v->data);

	free(v);
}

_Bool vector_insert(vector_t* v, size_t index, void* val) {
	if (v->size+1 > v->capacity)
		if (!vector_realloc(v, VECTOR_GROWTH(v->capacity)))
			return 0;

	char* o = v->data + (index * v->blksz);

	if (!memmove(o + v->blksz, o, v->blksz * (v->size - index)))
		return 0;

	memcpy(o, &val, v->blksz); /* don't feel like checking */
	v->size++;

	return 1;
}

_Bool vector_remove(vector_t* v, size_t index) {
	if (v->deleter)
		vector_call_deletere(v, index);

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

		do amnt = VECTOR_GROWTH(amnt);
		while (amnt < new_count);

		if (!vector_realloc(v, amnt))
			return 0;
	}

	/* super important, is it possible to just copy the adress of vals? */
	if (!memcpy(v->data + (v->size * v->blksz), /* & */ vals, v->blksz * val_count))
		return 0;

	v->size = new_count;

	return 1;
}

void vector_pop(vector_t* v, size_t i) {
	ssize_t diff = v->size - i;
	if (diff < 0)
		return;

	if (v->deleter)
		vector_call_deleteri(v, diff, i);

	v->size = v->size - i;
}

signed int vector_eq(vector_t* v1, vector_t* v2) {
	if (v1->size != v2->size || v1->blksz != v1->blksz)
		return v1->size - v2->size;

	for (size_t i = 0; i < v1->size; i++)
		if (memcmp(vector_at(v1, i), vector_at(v2, i), v1->blksz) != 0)
			return 1;

	return 0;
}

void* vector_set(vector_t* v, size_t index, void* val) {
	if (v->deleter)
		vector_call_deletere(v, index);

	return memcpy(v->data + (v->blksz * index), &val, v->blksz);
}

void vector_clear(vector_t* v) {
	if (v->deleter)
		vector_call_deletera(v);

	v->size = 0;
}

void vector_swap(vector_t* v1, vector_t* v2) {
	assert(v1->blksz == v2->blksz);
	ssize_t k = MIN(v1->size, v2->size);

	while (--k > 0)
		BSWAP(vector_at(v1, k), vector_at(v2, k), k * v1->blksz);
}
