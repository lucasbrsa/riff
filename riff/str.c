#include "str.h"

#include <stdio.h> /* can be removed once int2hex() is implemented */
#include <malloc.h>
#include <string.h>
#include <stdarg.h>

/* must use exclusively:
 * malloc
 * free
 * strlen
 * memcpy
 * memmove
 */

char* str_uits(unsigned __int64 u, char buf[__UI64_MAXS]) {
	static const char dpair_lut[] =
		"00010203040506070809"
		"10111213141516171819"
		"20212223242526272829"
		"30313233343536373839"
		"40414243444546474849"
		"50515253545556575859"
		"60616263646566676869"
		"70717273747576777879"
		"80818283848586878889"
		"90919293949596979899";

	char* end = buf + __UI64_MAXS - 1;
	int carry;
	*end = 0;

	while (u >= 100) {
		carry = ((u % 100) << 1) + 1;
		*--end = dpair_lut[carry--];
		*--end = dpair_lut[carry];
		u /= 100;
	}

	if (u < 10) {
		*--end = '0' + u;
		return end;
	}
	
	carry = ((u % 100) << 1) + 1;
	*--end = dpair_lut[carry--];
	*--end = dpair_lut[carry];
	u /= 100;
	return end;
}

char* str_sits(signed __int64 s, char buf[__SI64_MAXS]) {
	buf = str_uits(s, buf);

	if ((!!s) | -(int)((unsigned)s >> (63)))
		*--buf = '-';
	
	return buf;
}

char* str_rstrip(char* buf) {
	ssize_t len = strlen(buf);
	char* mbit = buf + len;

	do {
		mbit--;
	} while (len-- && STR_ISSPACE(*mbit));

	*(mbit + 1) = NULL;
	return buf;
}

char* str_lstrip(char* src) {
	while (STR_ISSPACE(*src) && *src++) {}

	return src;
}

char* str_strip(char* buf) {
	return str_lstrip(str_rstrip(buf));
}

str_ttble_t* str_gen_ttble(const char* keys, const char* vals) {
	/* assert(strlen(keys) == strlen(vals)) */

	str_ttble_t* t = malloc(sizeof(str_ttble_t));
	
	int it;
	for (it = 0; it < 256; it++) 
		t->lut[it] = it;
	
	while (*keys && *vals) t->lut[*keys++] = *vals++;

	return t;
}

char* str_trans_cpy(str_ttble_t* t, char* dest, const char* src) {
	char* d = dest;
	while(*src) {
		*d++ = t->lut[*src++];
	}

	*d = NULL;
	return dest;
}

char* str_trans(str_ttble_t* t, char* buf) {
	char* mbyte = buf;
	do {
		*mbyte = t->lut[*mbyte];
	} while (*mbyte++);

	return buf;
}

void str_free_ttble(str_ttble_t* t) {
	free(t);
}

char* str_filter(char* dest, const char* src, _Bool(func)(char)) {
	for (char* c = src; *c; c++)
		if ((*func)(*c))
			*dest++ = *c;
	
	*dest = NULL;
}

size_t str_find_substr(const char* src, const char* sub) {
	static str_tok_t ssrc = { NULL };
	if (src) { 
		ssrc.begoff = 0; 
		ssrc.tsrc = src; 
	}
	
	size_t sublen = strlen(sub), head = ssrc.begoff, tail;
	do {
		for (tail = 0; *(ssrc.tsrc + head + tail) == *(sub + tail); tail++) { }
		if (tail == sublen) {
			ssrc.begoff = head + tail;
			return head;
		}
	} while (*(ssrc.tsrc + head++));

	return NULL;
}

size_t str_find_substr_count(const char* src, const char* sub) {
	size_t cntr=0, l=strlen(sub),it;

	do {
		for (it = 0; *(src + it) == *(sub + it) && *(src + it); it++) { }
		if (it == l) { 
			cntr++; 
		} src += (it>0)?it:1;

	} while (*src);
	
	return cntr;
}

char* str_replace(char* dest, const char* src, const char* s1, const char* s2) {
	size_t l1 = strlen(s1), l2 = strlen(s2);
	char* d = dest, *s = (char*)src;

	do {
		size_t it;
		for (it = 0; *(s + it) == *(s1 + it) && *(s + it); it++) { }
		if (l1 - it) *d++ = *s;
		else { 
			memcpy(d, s2, l2); 
			d += l2; s += l1-1;
		}
	} while (*s++);

	return dest;
}

ssize_t str_search(const char* src, char t) {
	size_t i = 0;
	while (*(src + i++))
		if (*(src + i) == t)
			return i;
	return -1;
}

ssize_t str_dirsearch(const char* beg, const char* end, char t) {
	size_t i = 0;
	while ((beg + i++) != end)
		if (*(beg + i) == t)
			return i;

	return -1;
}

char* str_find_chr(char* src, char c) {
	while (*src)
		if (*++src == c)
			return src;
	
	return NULL;
}

ssize_t str_searcho(str_ostr_t src, ssize_t l, ssize_t r, char t) {
	if (r >= l) {
		ssize_t m = (r-l)/2+l;
		if (*(src+m) == t) return m;
		if (*(src+m) > t) return str_searcho(src, l, m - 1, t);
		else return str_searcho(src, m + 1, r, t);
	} 
	return -1;
}

size_t str_find_first(const char* src, const char* search) {
	for (size_t i = 0; *(src + i); i++)	
		if (str_search(search, *(src + i)) != -1) 
			return i; 

	return NULL;
}

size_t str_find_firstn(const char* src, const char* search) {
	for (size_t i = 0; *(src + i); i++)	
		if (!str_search(search, *(src + i)))
			return i; 

	return NULL;
}

size_t str_find_last(const char* src, const char* search) {
	size_t i = strlen(src);
	for (; i; i--) 
		if (str_search(search, *(src + i)))
			return i; 
	
	return NULL;
}

size_t str_find_lastn(const char* src, const char* search) {
	size_t i = strlen(src); 
	for (; i; i--) 
		if (!str_search(search, *(src + i)))
			return i; 
	
	return NULL;
}

char* str_toupper(char* buf) {
	char* b = buf;
	while (*b++)
		if (STR_ISLOWER(*b))
			*b &= '_';
	return buf;
}

char* str_tolower(char* buf) {
	char* b = buf;
	while (*b++)
		if (STR_ISUPPER(*b))
			*b |= ' ';
	return buf;
}

char* str_swap_case(char* buf) {
	char* b = buf;
	do {
		if (STR_ISLATIN(*b))
			*b ^= ' ';
	} while (*b++);

	return buf;
}

char* str_dircpy(char* dest, const char* src, ssize_t n, int step) {
	ssize_t len = strlen(src), 
		    astep = (step > 0)? step : step * -1;

	char *sptr = (step > 0)? src : src + len - 1, 
		 *dptr = dest;
	
	while (len > 0 && n--) {
		*dptr++ = *sptr;
		sptr += step; len -= astep;
	} 
	
	*dptr = NULL;
	return dest;
}

char* str_concat(size_t anct, char* dest, ...) {
	va_list args;
	va_start(args, dest);
	size_t end = 0, len;

	while (anct--) {
		char* carg = va_arg(args, const char*);
		len = strlen(carg);
		memcpy(dest + end, carg, len);
		end += len;
	}

	*(dest + end) = NULL;
	va_end(args);
	return dest;
}

char* str_mul(char* dest, const char* src, size_t rhs) {
	size_t slen = strlen(src);
	char* d = dest;

	while (rhs--) {
		memcpy(d, src, slen);
		d += slen;
	}

	*d = NULL;
	return dest;
}

char* str_dup(const char* src) {
	size_t len = strlen(src)+1;
	char* r = malloc(len);
	if (!r) return NULL;

	memcpy(r, src, len);
	return r;
}

char* str_lpad(char* dest, const char* src, size_t width, char pad_chr) {
	ssize_t s = strlen(src);
	if ((ssize_t)width - s <= 0)
		return memcpy(dest, src, s+1);
	
	memset(dest, pad_chr, width);
	memcpy(dest + (width - s), src, s+1);
	return dest;
}

char* str_rpad(char* dest, const char* src, size_t width, char pad_chr) {
	ssize_t s = strlen(src);
	if ((ssize_t)width - s <= 0)
		return memcpy(dest, src, s+1);

	memcpy(dest, src, s+1);
	memset(dest + s, pad_chr, (width - s));
	*(dest + width) = NULL;

	return dest;
}

char* str_cpad(char* dest, const char* src, size_t width, char pad_chr) {
	ssize_t s = strlen(src), l;
	if ((l = (ssize_t)width - s) <= 0)
		return memcpy(dest, src, s + 1);
	
	memset(dest, pad_chr, width);
	memcpy(dest + (l>>1), src, s);
	*(dest + width) = NULL;

	return dest;
}

char* str_tab_to_space(char* dest, const char* src, int w) {
	char* d = dest;
	do {
		if (*src == '\t') { 
			memset(d, ' ', w); 
			d += w; 
		} else *d++ = *src;
	} while (*src++);

	return dest;
}

char* str_space_to_tab(char* dest, const char* src, int c) {
	char* d = dest;
	size_t off = 0;
	
	do {
		int i;
		for (i = 0; *src == ' ' && i < c; i++, src++) { }
		if (i - c) {
			memcpy(d + off, src-i, i+1);
			off += i+1;
		} 
		else *(d + off++) = '\t', src--;
	} while (*src++);

	return dest;
}

const char* str_cescape(int c) {
	static str_etble_t str_etble = {
		"\\0","\\x01","\\x02","\\x03","\\x04","\\x05","\\x06","\\a","\\b","\\t","\\n","\\v","\\f","\\r","\\x0e","\\x0f",
		"\\x10","\\x11","\\x12","\\x13","\\x14","\\x15","\\x16","\\x17","\\x18","\\x19","\\x1a","\\x1b","\\x1c","\\x1d","\\x1e","\\x1f",
		" ","!","\\\"","#","$","%","&","\\'","(",")","*","+",",","-",".","/",
		"0","1","2","3","4","5","6","7","8","9",":",";","<","=",">","\\?",
		"@","A","B","C","D","E","F","G","H","I","J","K","L","M","N","O",
		"P","Q","R","S","T","U","V","W","X","Y","Z","[","\\\\","]","^","_",
		"`","a","b","c","d","e","f","g","h","i","j","k","l","m","n","o",
		"p","q","r","s","t","u","v","w","x","y","z","{","|","}","~","\\x7f",
	};

	/* assert(c < 128); */
	return str_etble[c];
}

char* str_cexescape(char* dest, int c) {
	if (c < 128)
		memcpy(dest, str_cescape(c), 5);
	else {
		memcpy(dest, "\\x", 2);
		sprintf(dest+2, "%02x", c);
	}

	return dest;
}

char* str_escape(char* dest, const char* src) {
	char* d = dest, k = 0;
	do {
		str_cexescape(dest + k, *src);
		const char* de = dest + k;
		int sit = 0;
		while (*de++ && sit++ < 5) k++;
	} while (*++src);

	return dest;
}

str_stble_t* str_splitn(const char* src, char delimiter, size_t max, str_stble_t* out) {
	size_t srlen = strlen(src)+1;
	out->refbuf = malloc(srlen);
	memcpy(out->refbuf, src, srlen);
	
	char *k, **it;
	for (k = out->refbuf, out->n = 0; k; out->n++)
		k = str_find_chr(k, delimiter);
	
	if (max && out->n > max)
		out->n = max;

	out->tble = it = malloc(sizeof(char*) * out->n);
	if (!it) 
		return NULL;

	int i;
	for (i = 0, k = out->refbuf; i < out->n; i++) {
		*it++ = k;
		k = str_find_chr(k, delimiter);
		
		if (out->n - i - 1)
			*k++ = NULL;
	}

	return out;
}

void str_free_split(str_stble_t* sres) {
	if (!sres) 
		return;
	if (sres->tble)
		free(sres->tble);
	if (sres->refbuf)
		free(sres->refbuf);
}

char* str_join(char* dest, char delimiter, const str_stble_t* in) {
	char* d = dest;
	for (int i = 0; i < in->n; i++) {
		size_t l = strlen(in->tble[i]);
		memcpy(d, in->tble[i], l);
		d += l;

		if (in->n - i - 1)
			*d++ = delimiter;
	} *d = NULL;

	return dest;
}

char* str_fmt_time(time_t t, char* dest, size_t destlen, const char* fmt) {
	/* idea for static time buf stolen from: https://github.com/happyfish100/libfastcommon */
	static char sbuf[64];
	
	if (dest) {
		dest = sbuf;
		destlen = sizeof(sbuf);
	}

	struct tm* tinf = localtime(&t);
	strftime(dest, destlen, fmt, tinf);
	
	return dest;
}

char* str_time_DDMMYYYY(time_t t, char* dest, size_t destlen) {
	struct tm* tinf = localtime(&t);
	snprintf(dest, destlen, "%02d%02d%04d", 
		tinf->tm_mday, tinf->tm_mon + 1, tinf->tm_year + 1900);

	return dest;
}

char* str_time_HHMMSS(time_t t, char* dest, size_t destlen) {
	struct tm* tinf = localtime(&t);
	snprintf(dest, destlen, "%02d%02d%02d",
		tinf->tm_hour, tinf->tm_min, tinf->tm_sec);

	return dest;
}

char* str_time_YYYYMMDDHHMMSS(time_t t, char* dest, size_t destlen) {
	struct tm* tinf = localtime(&t);
	snprintf(dest, destlen, "%04d%02d%02d%02d%02d%02d",
		tinf->tm_year + 1900, tinf->tm_mon + 1, tinf->tm_mday,
		tinf->tm_hour, tinf->tm_min, tinf->tm_sec);

	return dest;
}

size_t str_len_including(const char* src, const char* inclusions) {
	size_t l = 0;
	while (*src++)
		if (str_search(inclusions, *src) != -1)
			l++;

	return l;
}

size_t str_len_excluding(const char* src, const char* exclusions) {
	size_t l = 0;
	while (*src++)
		if (str_search(exclusions, *src) == -1)
			l++;

	return l;
}



/// @TODO
//char* str_reltoabs_path(char* dest, const char* rel, const char* path) {
//
//}

/// @TODO
//char* str_abstorel_path(char* dest, const char* rel, const char* path) {
//
//}

/// @TODO
//char* str_clean_path(char* dest, const char* src) {
//	/* replace \ with / */
//	/* remove ../ */
//	/* remove ./ */
//	/* add trailing / for directory and remove for file */
//	/* e.g. C:/users/Cameron/Desktop/\..\..\..\Windows\Web\..\..\Program Files\..\Program Files\..\.\Python27\tcl\.. */
//	/* ==> C:/Python27/ */
//}