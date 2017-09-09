#include <stdlib.h>
/* maybe update it to work with void*s rather than char*s */
size_t __str_bsearch(char* src, size_t l, size_t r, char t) {
	if (r >= l) {
		size_t m = (r - l) / 2 + l;

		if (src[m] == t) return m;

		if (src[m] > t) return __str_bsearch(src, l, m - 1, t);
		/*		*/ else return __str_bsearch(src, m + 1, r, t);
	}

	return -1;
}