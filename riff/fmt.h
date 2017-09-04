/* should be able to use input and output formats */
/* input fmt (fmti): allows a fmt to be compiled and the used to generate output from input data */
/* output fmt (fmto): allows for an action to be taken to produce data from a fmt stringlllllll */
#ifndef _FMT_H
#define _FMT_H

#include "generic.h"

/* each node in the buffer chain's length */
/* for short fmts a smaller number is appropriate */
/* #define FMT_BUFFERCHAIN_LEN 256 */

/* forward declaration */
struct fmt_t;
typedef struct fmt_t fmt_t;

/* a function pointer that defines each call in the symfunc_lut to be called in fmt_output from compiled fmt */
typedef void(*fmt_symfunc_f)(char* buf, void* itrpt_strct);

/* converts string format into a fmt_t which stores a set of function pointers for each format specifier */
/* both the fmt and symfunc_lut are used by the fmt_t, they cannot be destroyed til after the fmt_t is freed */
fmt_t* fmt_compile(const char* fmt, const fmt_symfunc_f symfunc_lut[26*2]);

/* free the memory allocated by fmt_compile */
void fmt_free(fmt_t* fmt);

/* given an itrnpt_strct and the compiled formation, make the accosiated calls */
void* fmt_output(fmt_t* f, void* itrnpt_strct);

#endif