#include "str.h"

#include <malloc.h>
#include <string.h>
#include <stdarg.h>

#define __RSTRIP(buffer, condition)		\
	ssize_t len = strlen(buffer);		\
	char* mbit = buffer + len;			\
	do { mbit--; if (!len--) break; }	\
	while (condition(*mbit));			\
	*(mbit + 1) = '\0';					\

#define __LSTRIP(buffer, logic)			\
	while (logic(*buffer) && *buffer) { \
		buffer++;						\
	}

void str_rstripf(char* buf, _Bool(func)(char)) {
	__RSTRIP(buf, (*func));
}

char* str_lstripf(char* buf, _Bool(func)(char)) {
	__LSTRIP(buf, (*func));
	return buf;
}

void str_rstrip(char* buf) {
	ssize_t len = strlen(buf);
	char* mbit = buf + len;
	do { mbit--; if (!len--) break; }
	while (__str_isspacec(*mbit));
	*(mbit + 1) = '\0';
}

char* str_lstrip(char* buf) {
	__LSTRIP(buf, __str_isspacec);
	return buf;
}

char* str_stripf(char* buf, _Bool(func)(char)) {
	__RSTRIP(buf, (*func));
	__LSTRIP(buf, (*func));
	return buf;
}

char* str_strip(char* buf) {
	__RSTRIP(buf, __str_isspacec);
	__LSTRIP(buf, __str_isspacec);
	return buf;
}

/* @TODO: use implementation from ruchir branch */
_Bool __str_bsearch(const char src[], ssize_t l, ssize_t r, char t) {
	if (r >= l) {
		ssize_t m = (r - l) / 2 + l;

		if (src[m] == t) return 1;

		if (src[m] > t) return __str_bsearch(src, l, m - 1, t);
		/*		*/ else return __str_bsearch(src, m + 1, r, t);
	}

	return 0;
}

_Bool __str_lsearch(const char src[], ssize_t so, char t) {
	for (; so; so--) {
		if (*(src + so) == t)
			return 1;
	}
	return 0;
}

#define __FILTER_C (c)
#define __FILTER(dest, src, condition)					\
	char* __FILTER_C;									\
	for (__FILTER_C = (char*)(src); *__FILTER_C; __FILTER_C++)	\
		if ((condition))								\
			*((dest)++) = *__FILTER_C;					\
	*(dest) = '\0';	

void str_filterf(char* dest, const char* src, _Bool(func)(char)) {
	__FILTER(dest, src, func(*__FILTER_C));
}

void str_filteru(char* dest, const char* src, const char filter[], size_t filter_len) {
	__FILTER(dest, src, !__str_lsearch(filter, filter_len, *__FILTER_C));
}

void str_filter(char* dest, const char* src, const char ordered_filter[], size_t filter_len) {
	__FILTER(dest, src, !__str_bsearch(ordered_filter, 0, filter_len - 1, *__FILTER_C));
}

void str_filterc(char* dest, const char* src, const char filter_char) {
	__FILTER(dest, src, *__FILTER_C != filter_char);
}

void str_toupper(char* buf) {
	while (*buf)
		if (str_islowerc(*buf))
			buf = buf - 'a' + 'A';
}

void str_tolower(char* buf) {
	while (*buf)
		if (str_isupperc(*buf))
			buf = buf - 'A' + 'a';
}

void str_swapcase(char* buf) {
	while (*buf) {
		if (str_isupperc(*buf)) buf -= 33;
		if (str_islowerc(*buf)) buf += 33;
	}
}

void str_dircpy(char* dest, const char* src, int step) {
	ssize_t len = (ssize_t)strlen(src);
	char *sptr = (step > 0)? (char*)src : (char*)src + len - 1, *dptr = dest;
	int astep = (step)?(step<0)?-1*step:step:1;
	while (len > 0) {
		*dptr++ = *sptr;
		sptr += step; len -= astep;
	}
	
	*dptr = '\0';
}

void str_concat(char* dest, ...) {
	va_list args;
	va_start(args, dest);
	size_t end = strlen(dest);

	char* carg;
	while(carg = va_arg(args, char*)) {
		strcpy(dest + end, carg);
		end += strlen(carg);
	}

	if (!(dest + end)) *(dest + end) = '\0';

	va_end(args);
}

void str_mul(char* dest, const char* src, size_t rhs) {
	size_t slen = strlen(src);
	while (rhs--) {
		strcpy(dest, src);
		dest += slen;
	}
}

char* str_dup(const char* src) {
	char* r = malloc(strlen(src) + 1);
	if (!r) return NULL;

	strcpy(r, src);
	return r;
}

#define _LPAD(buffer, src, pad_chr, l)\
	memset(buffer, pad_chr, l);		\
	strcpy(buffer + l, src);

#define _RPAD(buffer, src, pad_chr, s, w)	\
	strcpy(buffer, src);					\
	memset(buffer + s, pad_chr, (w-s));		\
	*(buffer + w) = '\0';

void str_lpadcpy(char* buf, const char* src, size_t width, char pad_chr) {
	ssize_t s = strlen(src);
	if ((width-s) <= 0) { strcpy(buf, src);	return; }
	_LPAD(buf, src, pad_chr, (width-s));
}

char* str_lpad(const char* src, size_t width, char pad_chr) {
	ssize_t s = strlen(src);
	if ((width-s) <= 0) return str_dup(src);
	char* buffer = malloc(width + 1);
	_LPAD(buffer, src, pad_chr, (width-s));
	return buffer;
}

void str_rpadcpy(char* buf, const char* src, size_t width, char pad_chr) {
	ssize_t s = strlen(src);
	if ((width-s) <= 0) { strcpy(buf, src);	return; }
	_RPAD(buf, src, pad_chr, s, width);
}

char* str_rpad(const char* src, size_t width, char pad_chr) {
	ssize_t s = strlen(src);
	if ((width-s) <= 0) return str_dup(src);
	char* buffer = malloc(width + 1);
	_RPAD(buffer, src, pad_chr, s, width);
	return buffer;
}

void str_cpadcpy(char* buf, const char* src, size_t width, char pad_chr) {
	ssize_t s = strlen(src), l;
	l = (width - s);
	if (l <= 0) {
		strcpy(buf, src);
		return;
	}
	_LPAD(buf, src, pad_chr, (l / 2));
	memset(buf + s + l / 2, pad_chr, l / 2 + (l % 2));
	*(buf + width) = '\0';
}

char* str_cpad(const char* src, size_t width, char pad_chr) {
	ssize_t s = strlen(src), l;
	l = (width - s);
	if (l <= 0) return str_dup(src);

	char* buffer = malloc(width + 1);
	_LPAD(buffer, src, pad_chr, (l / 2));
	memset(buffer + s + l / 2, pad_chr, l / 2 + (l % 2));
	*(buffer + width) = '\0';
	return buffer;
}

void str_tabtospace(char* buf, const char* src, int tab_width) {
	do {
		if (*src == '\t') {	memset(buf, ' ', tab_width); buf += tab_width; }
		else *buf++ = *src;
	} while (*src++);
}

/*
@ TODO
void str_spacetotab(char* buf, const char* src, int space_count) {
	char* spaces = malloc(space_count);
	memset(spaces, ' ', space_count);
	
	do {
		src = strstr(src, spaces);
	} while (strstr(src, spaces));

	free(spaces);
}
*/