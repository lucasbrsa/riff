// @TODO pass format modifiers to the callback
// @TODO make the fuckin thing compile
// @TODO make the fuckin thing link
// @TODO make the fuckin thing not crash
// @TODO make the fuckin thing work for one test case
// @TODO make the fuckin thing work for two test cases
// @TODO use hashmap for lut

/* should be able to use input and output formats */
/* input fmt (fmti): allows a fmt to be compiled and the used to generate output from input data */
/* output fmt (fmto): allows for an action to be taken to produce data from a fmt stringlllllll */
#ifndef _FMT_H
#define _FMT_H

#include "generic.h"

#include <stdio.h>
#include <malloc.h>

/* character that denotes format seq */
#define FMT_FMTCHR ('%')

/* a function pointer that defines each call in the symfunc_lut to be called in fmt_output from compiled fmt */
/* out is the buffer where text is written to */
/* return size_t the number of characters written to out */
/* itrpt_strct is any other data, may be cast and is passed to fmt_ouput */
typedef size_t(*fmt_symfunc_f)(char* out, void* itrpt_strct);

typedef struct fmt_t fmt_t;

#ifndef FMT_NCASE_SENS
typedef const fmt_symfunc_f fmt_symfunc_lut_t[26 * 2];
#else
typedef const fmt_symfunc_f fmt_symfunc_lut_t[26];
#endif

/* converts string format into a fmt_t which stores a set of function pointers for each format specifier */
/* both the fmt and symfunc_lut are used by the fmt_t, they cannot be destroyed til after the fmt_t is freed */
/* virgin '%'s in the fmt will cause nightmares, don't do it. use '%%' for a percent sign */
fmt_t* fmt_compile(const char* fmt, fmt_symfunc_lut_t lut);

/* free the memory allocated by fmt_compile */
void fmt_free(fmt_t* fmt);

/* given an itrnpt_strct and the compiled formation, make the accosiated calls */
char* fmt_output(const fmt_t* f, char* alloced_buf, void* itrnpt_strct);

/* wrap around fmt_output, printing the result */
#define fmt_print(f, stream, itrnpt_strct, l)			\
	do { char* intbuff = malloc(l);						\
	fputs(fmt_output(f, intbuff, itrnpt_strct), stream);\
	free(intbuff); } while(0)

#endif