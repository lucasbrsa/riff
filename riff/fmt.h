// @TODO pass format modifiers to the callback
// @TODO make the fuckin thing not crash
// @TODO make the fuckin thing work for one test case
// @TODO make the fuckin thing work for two test cases
// @TODO use hashmap for lut

/* should be able to use input and output formats */
/* input fmt (fmti): allows a fmt to be compiled and the used to generate output from input data */
/* output fmt (fmto): allows for an action to be taken to produce data from a fmt stringlllllll */
#ifndef _FMT_H
#define _FMT_H

#include "hashmap.h"
#include "generic.h"
#include "vector.h"

#include <stdio.h>
#include <malloc.h>

/* a function pointer that defines each call in the symfunc_lut to be called in fmt_output from compiled fmt */
/* out is the buffer where text is written to */
/* return size_t the number of characters written to out */
/* itrpt_strct is any other data, may be cast and is passed to fmt_ouput */
typedef size_t(*fmt_symfunc_f)(char* out, void* itrpt_strct);

/* the format stack is really just a vector of fmt_pair_t's */
typedef struct {
	vector_t* stack; /* std::vector<fmt_pair_t> */
} fmt_t;

/* converts string format into a fmt_t which stores a set of function pointers for each format specifier */
/* both lut and pattern are used within the fmt_stack_t, they cannot be destroyed til after free */
fmt_t* fmt_compile(const char* pattern, hashmap_t* lut /* std::unordered_map<fmt_symfunc_t*> */);

/* free the memory allocated by fmt_compile */
void fmt_free(fmt_t* fmt);

/* given an itrnpt_strct and the compiled formation, make the accosiated calls */
char* fmt_output(const fmt_t* fmt, char* alloced_buf, void* itrnpt_strct);

/* wrap around fmt_output, printing the result */
#define fmt_print(f, stream, itrnpt_strct, l)			\
	do { char* intbuff = malloc(l);						\
	fputs(fmt_output(f, intbuff, itrnpt_strct), stream);\
	free(intbuff); } while(0)

#endif