#include "fmt.h"
#include "str.h"

/* one section of the fmt stack must store... */
/* the callback for taht fmt flag, or NULL */
/* left is the pointer to the end of the previous specifier */
/* len is the precomputed length of the section of the string */
struct fmt_t {
	fmt_symfunc_f callback;
	char* left;
	size_t len;
};

fmt_t* fmt_compile(const char* fmt, fmt_symfunc_lut_t lut) {
	size_t depth = 0;
	for (char* c = fmt; *c; c++) {
		if (*c == FMT_FMTCHR)
			depth++, c++;
	}

	fmt_t* format_stack = malloc(sizeof(fmt_t) * depth + 1);
	format_stack[depth - 1].left = NULL; /* hmmm... */

	if (!format_stack)
		return free(format_stack), NULL;
	
	size_t head = 0;
	char *left_ptr, *itr;
	for (itr = left_ptr = fmt; *itr; itr++) {
		char fmt_chr = GEN_MAX(*(itr + 1), NULL);

		if (*itr == FMT_FMTCHR && fmt_chr && !STR_ISLATIN(fmt_chr)) {

#		ifndef FMT_NCASE_SENS
			fmt_chr = STR_ISLOWER(fmt_chr) ? fmt_chr - 'a' + 26 : fmt_chr - 'A';
#		else
			fmt_chr = STR_ISLOWER(fmt_chr) ? fmt_chr - 'a' : fmt_chr - 'A';
#		endif

			if (!lut[fmt_chr])
				continue;

			format_stack[head].left = left_ptr;
			format_stack[head].callback = lut[fmt_chr];

			if (head)
				format_stack[head - 1].len = (left_ptr - 2) - format_stack[head - 1].left;
			if (head - 1 == depth)
				format_stack[head].len = (*left_ptr) ? strlen(left_ptr) : 0;

			*itr = left_ptr = NULL;
			itr++, head++;
		}
		else if (!left_ptr)
			left_ptr = itr;
	}

	return format_stack;
}

void fmt_free(fmt_t* fmt) {
	if (fmt) {
		free(fmt);
	}
}

char* fmt_output(const fmt_t* f, char* alloced_buf, void* itrnpt_strct) {
	size_t it;
	char* rightedge = alloced_buf;
	for (it = 0; f[it].left; it++) {
		rightedge = (char*)memcpy(rightedge, f[it].left, f[it].len) + f[it].len;

		if (*f[it].callback)
			rightedge = rightedge + (*f[it].callback)(rightedge, itrnpt_strct);
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