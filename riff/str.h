#ifndef _STR_H
#define _STR_H

#include "generic.h"

#include <time.h>
#include <limits.h>
#include <float.h>

#define __STR(x) #x

#define __SI64_MAXS (10*sizeof(int64_t)*CHAR_BIT/33+3)

#define __UI64_MAXS (10*sizeof(uint64_t)*CHAR_BIT/33+2)

#define __PTR_MAXS (sizeof(__STR(SIZE_MAX))+1)

/* type conversion: signed int64_t to string */
char* str_sits(int64_t s, char buf[__SI64_MAXS]);

/* type conversion: unsigned int64_t to string */
char* str_uits(uint64_t u, char buf[__UI64_MAXS]);

/* strip trailing whitespace, returns point to the first non-whitespace char */
char* str_rstrip(char* buf);

/* strip leading whitespace */
char* str_lstrip(char* src);

/* strip trailing/leading whitespace */
char* str_strip(char* buf);

/* translation map */
typedef struct {
	char lut[256];
} str_ttble_t;

/* build a translation table */
str_ttble_t* str_gen_ttble(const char* keys, const char* vals);

/* copy in to out based on the translation table t */
char* str_trans_cpy(str_ttble_t* t, char* dest, const char* src);

/* modify mbuf based on the translation table t */
char* str_trans(str_ttble_t* t, char* buf);

/* free a translation table from memory */
void str_free_ttble(str_ttble_t* t);

/* filter chars out based on a function pointer */
char* str_filter(char* dest, const char* src, bool(func)(char));

/* stores information about previous calls to str_find_substr */
typedef struct {
	const char* tsrc; // <== be careful
	size_t begoff;
} str_tok_t;

/* find all occurrences of a sub string within a source string, call with src as NULL for next index */
size_t str_find_substr(const char* src, const char* sub);

/* find how often */
size_t str_find_substr_count(const char* src, const char* sub);

/* replace all instances of the substring s1 with s2 */
char* str_replace(char* dest, const char* src, const char* s1, const char* s2);

/* defines a string where each character is ordered */
typedef const char* str_ostr_t;

/* do a binary search through a ordered string src for instances of the char t, -1 on failure */
ssize_t str_searcho(str_ostr_t src, ssize_t l, ssize_t r, char t);

/* do a linear search through a string src for instances of the char t, -1 on failure */
ssize_t str_search(const char* src, char t);

/* do a linear backwards search from beg to end for instances of the char t, -1 on failure */
ssize_t str_dirsearch(const char* beg, const char* end, char t);

/* find instance of char within src */
char* str_find_chr(char* src, char c);

/* find the first character in src that matches any character of search */
size_t str_find_first(const char* src, const char* search);

/* find the first character in src that does not match any character of search */
size_t str_find_firstn(const char* src, const char* search);

/* find the last character in src that matches any character of search */
size_t str_find_last(const char* src, const char* search);

/* find the last character in src that does not match any character of search */
size_t str_find_lastn(const char* src, const char* search);

/* modify buf to become all upper-case */
char* str_toupper(char* buf);

/* modify buf to become all lower-case */
char* str_tolower(char* buf);

/* modify buf to flip the case of all latin characters */
char* str_swap_case(char* buf);

/* it is debated as to weather this should be included */
/* regardless it is implemented */
size_t str_cpy(char* dest, const char* src);

/* directional copy in place, like strncpy */
char* str_dircpy(char* dest, const char* src, ssize_t  n, int step);

/* allow for infinite string concatenation, the final argument must be NULL */
char* str_concat(size_t anct, char* dest, ...);

/* repeat a string src rhs times storing the result in dest */
char* str_mul(char* dest, const char* src, size_t rhs);

/* copy a string into a new malloc-ed buffer, emulation of POSIX implementation */
char* str_dup(const char* src);

/* left justify a string while copying to another buffer */
char* str_lpad(char* dest, const char* src, size_t width, char pad_chr);

/* right justify a string while copying to another buffer */
char* str_rpad(char* dest, const char* src, size_t width, char pad_chr);

/* center justify a string while copying to another buffer */
char* str_cpad(char* dest, const char* src, size_t width, char pad_chr);

/* convert all tab character to w spaces */
char* str_tab_to_space(char* dest, const char* src, int w);

/* convert all c spaces to a tab character */
char* str_space_to_tab(char* dest, const char* src, int c);

/* holds the statically allocated table of escape characters */
typedef const char str_etble_t[128][5];

/* convert chars to their associated escape character, for non extended ASCII chars */
const char* str_cescape(int c);

/* convert chars to their associated escape character, for all ASCII chars */
char* str_cexescape(char* dest, int c);

/* returned an escaped string, does not escape '\0' */
char* str_escape(char* dest, const char* src);

/* holds a string split table */
typedef struct {
	char* refbuf;
	char** tble;
	size_t n;
} str_stble_t;

/* break a delimited string into token, opposite of str_join, will allocate memory, will modify src, max to null for unlimited */
str_stble_t* str_splitn(const char* src, char delimiter, size_t max, str_stble_t* out);

/* free the allocated memory from str_split */
void str_free_split(str_stble_t* sres);

/* form a string int the format <a, b, c ... z>, opposite of str_split */
char* str_join(char* dest, char delimiter, const str_stble_t* in);

/* conversion from time_t to string based on a format string */
char* str_fmt_time(time_t t, char* dest, size_t destlen, const char* fmt);

/* format time with DDMMYY format */
char* str_time_DDMMYYYY(time_t t, char* dest, size_t destlen);

/* format time with HHMMSS format */
char* str_time_HHMMSS(time_t t, char* dest, size_t destlen);

/* format time with YYYYMMDDHHMMSS format */
char* str_time_YYYYMMDDHHMMSS(time_t t, char* dest, size_t destlen);

/* count character in a string that are present in inclusions */
size_t str_len_including(const char* src, const char* inclusions);

/* count character in a string that are not present in inclusions */
size_t str_len_excluding(const char* src, const char* exclusions);

/* make file paths follow consistent Linux style */
///char* str_clean_path(char* dest, const char* src);

/* make a relative file path absolute */
///char* str_reltoabs_path(char* dest, const char* rel, const char* path);

/* make an absolute file path relative */
///char* str_abstorel_path(char* dest, const char* rel, const char* path);

/* behaves like str_lpad, but padding with zero chars */
INLINE char* str_zfill(char* dest, const char* src, size_t width)
	{ return str_lpad(dest, src, width, '0'); }

/* reverse a string */
INLINE char* str_reverse(char* dest, const char* src)
	{ return str_dircpy(dest, src, -1, -1); }

/* replace all new lines with spaces */
INLINE char* str_newline_to_space(char* dest, const char* src)
	{ return str_replace(dest, src, "\n", " "); }

/* remove all instances of a substr */
INLINE char* str_remove(char* dest, const char* src, const char* sub)
	{ return str_replace(dest, src, sub, ""); }

/* directional copy in place, like strcpy */
INLINE char* str_dirncpy(char* dest, const char* src, int step)
	{ return str_dircpy(dest, src, -1, step); }

/* break a delimited string into token, opposite of str_join, will allocate memory, will modify src */
INLINE str_stble_t* str_split(const char* src, char delimiter, str_stble_t* out)
	{ return str_splitn(src, delimiter, 0, out); }

/* format time using internal static time buffer */
INLINE char* str_sfmt_time(time_t t, const char* fmt)
	{ return str_fmt_time(t, NULL, 0, fmt); }


#define STR_ISARABIC(c)		((c) >= '0' && (c) <= '9')

#define STR_ISLOWER(c)		((c) >= 'a' && (c) <= 'z')

#define STR_ISUPPER(c)		((c) >= 'A' && (c) <= 'Z')

#define STR_ISSPACE(c)		((c) == ' ' || ((c) >= '\x09' && (c) <= '\xD'))

#define STR_ISLATIN(c)		(STR_ISLOWER(c) || STR_ISUPPER(c))

#define STR_ISALPHANUM(c)	(STR_ISARABIC(c) || STR_ISLATIN(c))

#endif
