#include "log.h"
#include "str.h"

#include <time.h>
#include <string.h>
#include <stdlib.h>

#define TBUFFLEN 0xFF
#define MBUFFLEN 0x200

log_fmt_t log_compile_pattern(const char* fmt) {
	/* first pass to determine number of members, can be removed once vector is implemented */
	char* itr = fmt;
	int sz = 1;
	do {
		if (*itr == '%' && *(itr + 1) && *(itr + 1) != '%')
			sz++;
	} while (*++itr);


	log_fmt_t res = {
		.stack = malloc(sizeof(log_fmt_stpair_t) * sz),
		.left_pool = str_dup(fmt),
		.len = sz,
	};
	
	unsigned idx = 0;
	char* prev_ptr, itr;
	itr = prev_ptr = res.left_pool;
	for (; itr; itr++) {
		if (*itr == '%' && *(itr + 1) && *(itr + 1) != '%') {
			if (prev_ptr) {
				res.stack[idx].leftof = prev_ptr;
				prev_ptr = 0;
			}

			if (*++itr)	
				res.stack[idx].c = log_interpret_fmt_flag(*itr);
			else 
				break;

			*itr = 0; idx++;
		} 
		else 
			if (!prev_ptr)
				prev_ptr = itr;
	}

	res.stack[idx].leftof = prev_ptr;
	res.stack[idx].c = 0;

	return res;
}

void log_free_fmt(log_fmt_t patt) {
	if (&patt) {
		free(patt.left_pool);
		free(patt.stack);
	}
}

void log_print_fmt(log_message_t* msg, log_fmt_t fmt) {
	char* fb = malloc(MBUFFLEN); /* it would be cool if there was a way of doing things independent of length... like a linked list of buffers */
	msg->formatted = fb;

	for (int i = 0; i < fmt.len; i++)
		memcpy(msg->formatted, fmt.stack[i].leftof, (i < fmt.len) ? fmt.stack[i + 1].leftof - fmt.stack[i].leftof : strlen(fmt.stack[i + 1].leftof)), fmt.stack[i].c(msg);

	puts(fb);
	free(fb);
}

void log_interpret_flag_c(log_message_t* msg);
void log_interpret_flag_m(log_message_t* msg);
void log_interpret_flag_t(log_message_t* msg);

log_fmt_cback log_interpret_fmt_flag(char flg) {
	static log_fmt_cback cback_lut[26] = {
		0, 0, log_interpret_flag_c, 0, 0, 0, 0, 0, 0, 0, 0, 0, log_interpret_flag_m, 0, 0, 0, 0, 0, 0, log_interpret_flag_t, 0, 0, 0, 0, 0, 0
	};

	/* could potentially require switch for certain custom interprets */
	return (str_islower(flg))? cback_lut[flg] : 0;
}

void log_interpret_flag_c(log_message_t* msg) {
	static const char* catlut = "\x03\x08\x0Dblah\0blah\0"; /* example of cunty hard coded catagories */

	size_t d = catlut[msg->cat + 1] - catlut[msg->cat];
	memcpy(msg->formatted, catlut[catlut[msg->cat]], d);
	msg->formatted += d;
}

void log_interpret_flag_m(log_message_t* msg) {
	size_t d = strlen(msg->cat);
	memcpy(msg->formatted, msg->cat, d);
	msg->formatted += d;
}

void log_interpret_flag_t(log_message_t* msg) {
	/* just as example you ass hat */
	time_t rawtime; time(&rawtime);
	char* buff = malloc(TBUFFLEN);
	strftime(buff, TBUFFLEN, "%x %X", localtime(&rawtime));
}