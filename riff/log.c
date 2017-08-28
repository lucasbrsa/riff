#include "log.h"
#include "str.h"

#include <time.h>
#include <string.h>
#include <stdlib.h>

#define LOG_TBL		32
#define LOG_BMSGL	128

log_fmt_t log_compile_pattern(const char* fmt) {
	/* first pass to determine number of members, can be removed once vector is implemented */
	char* itr = fmt;
	int sz = 0;
	do {
		if (*itr == '%' && *(itr + 1)) {
			if (*(itr + 1) != '%') 
				sz++;
			else 
				itr++;
		}
	} while (*++itr);


	log_fmt_t res = {
		.stack = malloc(sizeof(log_fmt_stpair_t) * sz), /* allocation */
		.left_pool = str_dup(fmt), /* allocation */
		.len = sz,
	};
	
	unsigned idx = 0;
	char* prev_ptr;
	for (itr = prev_ptr = res.left_pool; *itr; itr++) {
		if (*itr == '%' && *(itr + 1) && *(itr + 1) != '%') {
			res.stack[idx].leftof = prev_ptr;
			res.stack[idx].c = log_interpret_fmt_flag(*(1+itr));
			*itr = 0; prev_ptr = 0; idx++, itr++;
		} 
		else 
			if (!prev_ptr)
				prev_ptr = itr;

		if (*itr == '%' && *(itr + 1) == '%')
			itr++;
	}

	res.stack[idx].leftof = prev_ptr;
	res.stack[idx].c = 0;

	return res;
}

void log_free_fmt(log_fmt_t* const patt) {
	if (patt) {
		free(patt->left_pool);
		free(patt->stack);

		patt->left_pool = NULL;
		patt->stack = NULL;
	}
}

void log_print_fmt(log_msg_t* msg, log_fmt_t fmt) {
	char* fb = malloc(LOG_BMSGL); /* it would be cool if there was a way of doing things independent of length... like a linked list of buffers */
	msg->out = fb;

	for (int i = 0; i < fmt.len; i++) {
		size_t mlen = ((i + 1 < fmt.len) ?
			fmt.stack[i + 1].leftof - fmt.stack[i].leftof :
			strlen(fmt.stack[i].leftof)) - 2;

		msg->out = (char*)memcpy(msg->out, fmt.stack[i].leftof, mlen) + mlen;
		fmt.stack[i].c(msg);
	}

	puts(fb);

	puts(fb);
	free(fb);
	msg->out = NULL; /* just to be safe, not really nessisary though */
}

void log_interpret_flag_c(log_msg_t* msg);
void log_interpret_flag_m(log_msg_t* msg);
void log_interpret_flag_t(log_msg_t* msg);

/* consider inlining this function into log_compile_pattern since it's so fuckin simple */
log_fmt_cback log_interpret_fmt_flag(char flg) {
	static log_fmt_cback cback_lut[26] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, log_interpret_flag_m, 0, 0, 0, 0, 0, 0, log_interpret_flag_t, 0, 0, 0, 0, 0, 0
	};

	/* could potentially require switch for certain custom interprets */
	return (str_islower(flg))? cback_lut[flg - 'a'] : 0;
}

void log_interpret_flag_m(log_msg_t* msg) {
	size_t d = strlen(msg->in);
	memcpy(msg->out, msg->in, d);
	msg->out += d-1;
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