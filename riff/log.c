#include <assert.h>
#include <malloc.h>
#include <assert.h>

#include "hashmap.h"
#include "log.h"
#include "str.h"

void log_itrprt_m(log_msg_t* msg) {
	const char* i = msg->in;
	do {
		*msg->out++ = *i;
	} while (*++i);
}

log_fmt_t* log_compile_pattern(const char* fmt) {
	const log_fmt_mod_fun lut[26] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, log_itrprt_m,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	};

	log_fmt_t* f = malloc(sizeof(log_fmt_t));
	f->pool = str_dup(fmt);
	f->stack = vector_init(8, sizeof(log_fmt_pair_t*), free);

	char *left_ptr = NULL, *itr;
	for (itr = f->pool; *itr; itr++) {
		if (*itr == '%' || !*(itr + 1)) {
			if (left_ptr) {
				/* push a string to the stack */
				log_fmt_pair_t* p = malloc(sizeof(log_fmt_pair_t));
				p->type = LOG_FMT_STRR;

				p->pool_ptr = left_ptr;

				left_ptr = NULL; *itr = 0;
				vector_push_back(f->stack, &p);
			}

			if (*++itr) {
				/* push a callback to the stack */
				log_fmt_pair_t* p = malloc(sizeof(log_fmt_pair_t));
				p->type = LOG_FMT_CBAK;

				assert(STR_ISLOWER(*itr)); // < ---
				assert(lut[*itr - 'a'] != 0); // < ---

				p->fun = lut[*itr - 'a']; // <-- DEAL WITH THIS SHIT LATER...
				vector_push_back(f->stack, &p);
			} else
				break;
		} else {
			if (!left_ptr)
				left_ptr = itr;
		}
	}

	return f;
}

void log_free_pattern(log_fmt_t* f) {
	if (f) {
		vector_free(f->stack);
		free(f->pool);
		free(f);
	}
}

void log_test(log_fmt_t* f, log_msg_t* m) {
	char* beg = m->out = malloc(128);
	for (void* it = vector_front(f->stack); it != vector_back(f->stack); it = vector_next(f->stack, it)) {
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

