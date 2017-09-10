#include <assert.h>

#include "fmt.h"
#include "str.h"

/* unions cannot store their current type (for fuck's sake dennis) */
enum fmt_utype {
	FMT_UTYPE_STRING, FMT_UTYPE_CALLBACK
};

/* str fmt */
typedef struct {
	char* data;
	size_t len;
} fmt_str;

/* any member of the fmt vector may be one of the two m types */
typedef struct {
	union {
		fmt_symfunc_f callback;
		fmt_str left;
	} m;
	enum fmt_utype t;
} fmt_pair_t;


char* fmt_output(const fmt_t* fmt, char* alloced_buf, void* itrnpt_strct);

fmt_t* fmt_compile(const char* pattern, hashmap_t* lut) {
	fmt_t* f = malloc(sizeof(f));
	vector_t* fmtv = vector_init(8, sizeof(fmt_pair_t), NULL);
	f->stack = fmtv;

	if (!f || !fmtv)
		return NULL;

	char *left_ptr, *itr;
	for (itr = left_ptr = pattern; *itr; itr++) {
		if (*itr == '%') {
			if (left_ptr) {
				fmt_str s = { .data = left_ptr,.len = itr - left_ptr };
				fmt_pair_t p = (fmt_pair_t){
					.m = &s, .t = FMT_UTYPE_STRING };
				vector_push_back(fmtv, &p);
				*itr = left_ptr = NULL;
			}

			if (*++itr) {
				fmt_pair_t p = { .m = hashmap_get(&lut, itr), .t = FMT_UTYPE_CALLBACK };
				vector_push_back(fmtv, &p);
			}
			else
				break;
		}
		else {
			if (!left_ptr)
				left_ptr = itr;
		}
	}

	return f;
}

void fmt_free(fmt_t* fmt) {
	if (fmt) {
		vector_free(fmt->stack);
		free(fmt);
	}
}

char* fmt_output(const fmt_t* f, char* alloced_buf, void* itrnpt_strct) {
	for (int i = 0; i < f->stack->size; i++) {
		fmt_pair_t p = *(fmt_pair_t*)vector_at(f->stack, i);

		switch (p.t) {
			case FMT_UTYPE_STRING:
				alloced_buf = memcpy(alloced_buf, p.m.left.data, p.m.left.len);
				break;
			case FMT_UTYPE_CALLBACK:
				alloced_buf += p.m.callback(alloced_buf, itrnpt_strct);
				break;
		}
	}
	
	return alloced_buf;
}

#if 0

void log_print_fmt(log_msg_t* msg, log_fmt_t fmt) {
	/* msg->out = vector_init(LOG_BMSGL, sizeof(char)); */
	msg->out = malloc(LOG_BMSGL);

	for (int i = 0; i < fmt.depth; i++) {
		size_t mlen;

		if (i < fmt.depth - 2)
			mlen = fmt.stack[i + 1].leftof - fmt.stack[i].leftof;
		else
			mlen = strlen(fmt.stack[i].leftof);

		msg->out = (char*)memcpy(msg->out, fmt.stack[i].leftof, mlen) + mlen;
		if (fmt.stack[i].c)
			fmt.stack[i].c(msg);
	}

	puts(msg->out);
	free(msg->out);
	msg->out = NULL; /* just to be safe, not really nessisary though */
}

log_fmt_t log_compile_pattern(const char* fmt) {
	static log_fmt_cback cback_lut[26] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, log_interpret_flag_m, 0, 0, 0, 0, 0, 0, log_interpret_flag_t, 0, 0, 0, 0, 0, 0
	};

	size_t depth = 0;
	for (char* c = fmt; *c; c++) {
		if (*c == '%' && *(c + 1) && *(c + 1) != '%')
			depth++, c++;
	}

	log_fmt_t res = {
		.stack = malloc(sizeof(log_fmtsec_t) * depth), /* stores the fmt pairs */
		.pool = str_dup(fmt), /* this is where the fmt is allocated */
		.depth = depth
	};
	
	size_t head = 0;
	char *prev_ptr, *itr;
	for (itr = prev_ptr = res.pool; *itr; itr++) {
		if (*itr == '%' && *(itr + 1) && *(itr + 1) != '%') {
			res.stack[head].leftof = prev_ptr;
			res.stack[head].c = (str_islower(*(1 + itr))) ? cback_lut[*(1 + itr) - 'a'] : 0;
			
			if (head)
				res.stack[head - 1].llen = res.stack[head].leftof - res.stack[head - 1].leftof;
			if (head == res.depth)
				res.stack[head].llen = strlen(res.stack[head].leftof); /* maybe -1 head or some shit */

			*itr = prev_ptr = 0;
			itr++, head++;
		} 
		else {
			if (!prev_ptr)
				prev_ptr = itr;
		}
		
		/* handles %% case, look for a neater solution */
		/* will not make %% = % rather %% = %% fix */
		if (*itr == '%' && *(itr + 1) == '%')
			itr++;
	}

	res.stack[res.depth - 1].leftof = (prev_ptr) ? prev_ptr : itr;
	res.stack[res.depth - 1].c = 0;

	return res;
}

void log_free_fmt(log_fmt_t* const patt) {
	if (patt) {
		free(patt->stack);
		free(patt->pool);

		patt->pool = NULL;
		patt->stack = NULL;
	}
}

#endif