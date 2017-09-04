#include "log.h"
#include "str.h"

#include <time.h>
#include <string.h>
#include <stdlib.h>

void log_interpret_flag_m(log_msg_t* msg);
void log_interpret_flag_t(log_msg_t* msg);

#define LOG_DFMTC	8
#define LOG_TBL		32
#define LOG_BMSGL	128

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

void log_interpret_flag_m(log_msg_t* msg) {
	size_t d = strlen(msg->in);
	msg->out = (char*)memcpy(msg->out, msg->in, d) + d;
}

void log_interpret_flag_t(log_msg_t* msg) {
	/* just as example you ass hat */
	time_t rawtime; time(&rawtime);
	char* buff = malloc(LOG_TBL);
	int l;

	strftime(buff, LOG_TBL, "%x %X", localtime(&rawtime));
	l = strlen(buff);

	memcpy(msg->out, buff, l);
	msg->out += l - 1;

	free(buff);
}