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
			*itr = 0; prev_ptr = 0; idx++;
		} 
		else 
			if (!prev_ptr)
				prev_ptr = itr;

		if (*itr == '%' && *(itr + 1) == '%')
			itr++;
	}

	res.stack[idx].leftof = prev_ptr;
	res.stack[idx].c = 0;

	for (int i = 0; i < res.len; i++) {
		printf("%s", res.stack[i].leftof);
	}

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
	char* fb = malloc(MBUFFLEN); /* it would be cool if there was a way of doing things independent of length... like a linked list of buffers */
	msg->_formatted = fb;

	for (int i = 0; i < fmt.len; i++) {
		memcpy(msg->_formatted, fmt.stack[i].leftof, (i + 1 < fmt.len) ?
			fmt.stack[i + 1].leftof - fmt.stack[i].leftof :
			strlen(fmt.stack[i].leftof));
		fmt.stack[i].c(msg);
	}

	puts(fb);
	free(fb);
	msg->_formatted = NULL; /* just to be safe, not really nessisary though */
}

void log_interpret_flag_c(log_msg_t* msg);
void log_interpret_flag_m(log_msg_t* msg);
void log_interpret_flag_t(log_msg_t* msg);

/* consider inlining this function into log_compile_pattern since it's so fuckin simple */
log_fmt_cback log_interpret_fmt_flag(char flg) {
	static log_fmt_cback cback_lut[26] = {
		0, 0, log_interpret_flag_c, 0, 0, 0, 0, 0, 0, 0, 0, 0, log_interpret_flag_m, 0, 0, 0, 0, 0, 0, log_interpret_flag_t, 0, 0, 0, 0, 0, 0
	};

	/* could potentially require switch for certain custom interprets */
	return (str_islower(flg))? cback_lut[flg - 'a'] : 0;
}

void log_interpret_flag_c(log_msg_t* msg) {
	static const char* catlut = "\x03\x08\x0d" "blah\0blah"; /* example of cunty hard coded catagories */

	size_t d = catlut[msg->cat + 1] - catlut[msg->cat];
	memcpy(msg->_formatted, /* *(catlut + *(catlut + msg->cat))*/ "1234", d);
	msg->_formatted += d;
}

void log_interpret_flag_m(log_msg_t* msg) {
	size_t d = strlen(msg->message);
	memcpy(msg->_formatted, msg->message, d);
	msg->_formatted += d;
}

void log_interpret_flag_t(log_msg_t* msg) {
	/* just as example you ass hat */
	time_t rawtime; time(&rawtime);
	char* buff = malloc(TBUFFLEN);
	strftime(buff, TBUFFLEN, "%x %X", localtime(&rawtime));
}