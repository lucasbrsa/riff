#ifndef _STR_H
#define _STR_H

/* if the param is an ssize_t as opposed to a size_t, -1 can be parsed to ignore the len */

#include "macro.h"

/* string manipulation */

/* strip trailing/leading whitespace */
void str_rstripf(char* buf, _Bool(func)(char)); /* O(n) */
void str_rstrip(char* buf);						/* O(n) */
char* str_lstripf(char* buf, _Bool(func)(char));/* O(n) */
char* str_lstrip(char* buf);					/* O(n) */
char* str_stripf(char* buf, _Bool(func)(char));	/* O(n) */
char* str_strip(char* buf);						/* O(n) */

/* filter out certain chars */
/* filter_arr must be an ordered set to allow for O(n log(n)) filtering */
void str_filterf(char* dest, const char* src, _Bool(func)(char));						/* O(n * f) */
void str_filterc(char* dest, const char* src, const char filter_char);					/* O(n) */
void str_filteru(char* dest, const char* src, const char filter[], size_t filter_len);	/* O(n * m)) */
void str_filter(char* dest, const char* src, const char ordered_filter[], size_t filter_len);/* O(n log(m)) */

/* add translate */
/* add replace */

/* change case */
/* because strlwr and strupr are depreciated */
void str_toupper(char* buf);  /* O(n) */
void str_tolower(char* buf);  /* O(n) */

/* swap cases */
void str_swapcase(char* buf);  /* O(n) */

/* directional copy in place, like strcpy */
//void str_dircpyn(char* dest, const char* src, size_t len, int step);
void str_dircpy(char* dest, const char* src, int step);  /* O(n) */

/* allow for infinite strcat */
/* final argument MUST be '\0' or NULL */
void str_concat(char* dest, ...);  /* O(n * m) */

/* string multiplication */
void str_mul(char* dest, const char* src, size_t rhs);  /* O(n * m) */

/* as it is not provided by all stdlib implementations, must be freed */
char* str_dup(const char* src);

/* justify a string while copying to another buffer */
void str_lpadcpy(char* buf, const char* src, size_t width, char pad_chr); /* O(n) ALSO NPM 'left-pad' IS NOT FUCKIN O(log(n)) STRING CONCATS ARE NOT CONSTANT TIME YOU FUCKING RETARDS */
void str_rpadcpy(char* buf, const char* src, size_t width, char pad_chr); /* O(n) */
void str_cpadcpy(char* buf, const char* src, size_t width, char pad_chr); /* O(n) */

/* add justify funcs */

/* justify a string, while allocating a new buffer, must be freed */
/* do NOT pass output directly to puts/printf this will create a memory leak */
/* store the output in a variable and call free() once usage is finished */
char* str_lpad(const char* src, size_t width, char pad_chr); /* O(n) */
char* str_rpad(const char* src, size_t width, char pad_chr); /* O(n) */
char* str_cpad(const char* src, size_t width, char pad_chr); /* O(n) */

/* misc */
void str_tabtospace(char* buf, const char* src, int tab_width);
void str_spacetotab(char* buf, const char* src, int space_count); /* not very efficient */
void str_join(char* buf, const char* delimiter, const char** iarr, size_t iarr_len);
void str_joinn(char* buf, size_t len, const char* delimiter, const char** iarr, size_t iarr_len);
char* str_escape(char c);

/* string examination */

/* sub strings */
int str_substrcount(const char* src);
int str_substrcountn(const char* src, size_t len);

/* boolean operations */
#define __str_isarabicc(c) ((c) >= '0' && (c) <= '9')
#define __str_islowerc(c) ((c) >= 'a' && (c) <= 'z')
#define __str_isupperc(c) ((c) >= 'A' && (c) <= 'Z')
#define __str_isspacec(c) ((c) == ' ' || ((c) >= '\x09' && (c) <= '\xD'))
#define __str_islatinc(c) ( __str_islowerc(c) || __str_isupperc(c))
#define __str_isalphanumc(c) ( __str_isarabicc(c) || __str_islatinc(c))

/* wrappers */
INLINE void str_zfill(char* buf, const char* src, size_t width) { str_lpadcpy(buf, src, width, '0'); }
INLINE void str_reverse(char* buf) { str_dircpy(buf, buf, -1); }
INLINE _Bool str_isarabicc(char c) { return __str_isarabicc(c); }
INLINE _Bool str_islowerc(char c) { return __str_islowerc(c); }
INLINE _Bool str_isupperc(char c) { return __str_isupperc(c); }
INLINE _Bool str_isspacec(char c) { return __str_isspacec(c); }
INLINE _Bool str_islatinc(char c) { return __str_islatinc(c); }
INLINE _Bool str_isalphanumc(char c) { return __str_isalphanumc(c); }

// add more that you think are necessary
// do not implement functions that already exist in the std c lib
// ...

#endif
