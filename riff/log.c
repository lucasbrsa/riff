#include "log.h"

log_cfmt_t log_compile_pattern(const char* fmt) {
	log_cfmt_t c;

	/* first pass to determine number of members, can be removed once vector is implemented */
	char* iitr = fmt;
	int sz = 0;
	do {
		if (*iitr == '%' && *(iitr + 1) && *(iitr + 1) != '%')
			sz++;
	} while (*++iitr);
	sz = sz * 2 + 1;

	c.lut = malloc(sizeof(c.lut) * sz);

	iitr = fmt;
	for (; iitr; iitr++) {
		if (*iitr == '%' && *(iitr + 1) && *(iitr + 1) != '%') {

		}
	}
}