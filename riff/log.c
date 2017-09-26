#include <assert.h>
#include <malloc.h>
#include <assert.h>

#include "hashmap.h"
#include "log.h"
#include "str.h"

/* implementations of format specifiers */

void __m(log_msg_t* msg) {
	msg->out += str_cpy(msg->out, msg->in);
}

void __t(log_msg_t* msg) {
	/* use static time buffers */
	static char stbuf[26];

	strftime(stbuf, 26, "%Y-%m-%d %H:%M:%S", msg->tinfo);
	msg->out += str_cpy(msg->out, stbuf);
}

void __percent (log_msg_t* msg) {
	*msg->out++ = '%';
}

static log_fmt_mod_fun flut[26 * 2 + 1] = {
	/* uppercase */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

	/* lowercase */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, __m,
	0, 0, 0, 0, 0, 0, __t, 0, 0, 0, 0, 0, 0,

	/* %% */
	__percent
};

log_fmt_t log_compile_pattern(const char* fmt) {
	log_fmt_t f;
	f.pool = str_dup(fmt);
	f.stack = vector_init(8, sizeof(log_fmt_pair_t*), free);

	char *left_ptr = NULL, *itr;
	for (itr = f.pool; *itr; itr++) {
		if (*itr == '%' || !*(itr + 1)) {

			/* push a string to the stack */
			if (left_ptr) {
				log_fmt_pair_t* p = malloc(sizeof(log_fmt_pair_t));
				p->type = LOG_FMT_STRR; p->pool_ptr = left_ptr;

				vector_push_back(f.stack, &p);
				left_ptr = NULL; *itr = 0;
			}

			/* push a callback to the stack */
			if (*++itr) {
				if (!STR_ISLATIN(*itr) && *itr != '%')
					continue;

				char index = (STR_ISUPPER(*itr))?*itr-'A':(STR_ISLOWER(*itr))?*itr-'a'+26:26*2;

				if (!flut[index])
					continue;

				log_fmt_pair_t* p = malloc(sizeof(log_fmt_pair_t));
				p->type = LOG_FMT_CBAK; p->fun = flut[index];

				vector_push_back(f.stack, &p);
			} else
				break;
		} else
			if (!left_ptr)
				left_ptr = itr;
	}

	return f;
}

void log_free_pattern(log_fmt_t f) {
	vector_free(f.stack);
	free(f.pool);
}

void log_test(log_fmt_t f, log_msg_t* m) {
	char* beg = m->out = malloc(128);
	for (void* it = vector_front(f.stack); it != vector_back(f.stack); it = vector_next(f.stack, it)) {
		log_fmt_pair_t tmp = **(log_fmt_pair_t**)(it);

		assert(tmp.type == LOG_FMT_STRR || tmp.type == LOG_FMT_CBAK);

		if (tmp.type == LOG_FMT_CBAK)
			tmp.fun(m);

		if (tmp.type == LOG_FMT_STRR) {
			const char* i = tmp.pool_ptr;
			do {
				*m->out++ = *i;
			} while (*++i);
		}
	}

	m->out = beg;
}
