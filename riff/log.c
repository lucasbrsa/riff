#include <assert.h>
#include <malloc.h>
#include <assert.h>

#include "hashmap.h"
#include "log.h"
#include "str.h"

/* globals */
static log_fmt_t* global_fmt = NULL;
static hashmap_t* global_lmap = NULL;

/* implementations of format specifiers:
 *
 * %d default format
 * %f file the log was generated in
 * %F function the log was generated in
 * %i the message number (id)
 * %l line the log was generated on
 * %m the log message
 * %n the logger name
 * %p log priority
 * %P short log priority
 * %t full formatted date time
 * %% percent sign
*/

void __d(log_msg_t* msg) {
	static log_fmt_t* f = NULL;

	if (!f)
		f = log_compile_pattern("[%n] %m");

	log_format(f, msg);
}

void __f(log_msg_t* msg) {
	msg->formatted += str_cpy(msg->formatted, msg->file);
}

void __F(log_msg_t* msg) {
	msg->formatted += str_cpy(msg->formatted, msg->func);
}

void __i(log_msg_t* msg) {
	static char lbuf[__UI64_MAXS];
	char* spos = str_uits(msg->id, lbuf);
	msg->formatted += str_cpy(msg->formatted, spos);
}

void __l(log_msg_t* msg) {
	static char lbuf[__UI64_MAXS];
	char* spos = str_uits(msg->line, lbuf);
	msg->formatted += str_cpy(msg->formatted, spos);
}

void __m(log_msg_t* msg) {
	msg->formatted += str_cpy(msg->formatted, msg->message);
}

void __n(log_msg_t* msg) {
	msg->formatted += str_cpy(msg->formatted, msg->lname);
}

void __p(log_msg_t* msg) {
	static char* lut[] = {
		"trace",
		"debug",
		"info",
		"warn",
		"error",
		"crit"
	};

	assert(msg->priority >= 0 && msg->priority < (sizeof(lut) / sizeof(char*)));
	msg->formatted += str_cpy(msg->formatted, lut[msg->priority]);
}

void __s(log_msg_t* msg) {
	static const char* lut = "LDIWEC";

	assert(msg->priority >= 0 && msg->priority < sizeof("LDIWEC"));
	*msg->formatted++ = *(lut + msg->priority);
}

void __t(log_msg_t* msg) {
	static char stbuf[26];

	/* replace with global time format */
	strftime(stbuf, 26, "%Y-%m-%d %H:%M:%S", msg->tinfo);
	msg->formatted += str_cpy(msg->formatted, stbuf);
}

void __percent (log_msg_t* msg) {
	*msg->formatted++ = '%';
}

log_fmt_t* log_compile_pattern(const char* fmt) {

	static log_fmod_f flut[26 * 2 + 1] = {
		/* uppercase */
		0, 0, 0, 0, 0, __F, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

		/* lowercase */
		0, 0, 0, __d, 0, __f, 0, 0, __i, 0, 0, __l, __m,
		__n, 0, __p, 0, 0, __s, __t, 0, 0, 0, 0, 0, 0,

		/* %% */
		__percent
	};

	log_fmt_t* f = malloc(sizeof(log_fmt_t));
	f->stack = vector_init(8, sizeof(log_fpair_t*), free);
	f->pool = str_dup(fmt);

	char *left_ptr = NULL, *itr;
	for (itr = f->pool; *itr; itr++) {
		if (*itr == '%' || !*(itr + 1)) {

			if (left_ptr) {
				log_fpair_t* p = malloc(sizeof(log_fpair_t));
				p->type = LOG_FMT_STRR; p->pool_ptr = left_ptr;

				vector_push_back(f->stack, &p);
				left_ptr = NULL; *itr = 0;
			}

			if (*++itr) {
				if (!STR_ISLATIN(*itr) && *itr != '%')
					continue;

				char index = (STR_ISUPPER(*itr))?*itr-'A':(STR_ISLOWER(*itr))?*itr-'a'+26:26*2;

				if (!flut[(unsigned)index])
					continue;

				log_fpair_t* p = malloc(sizeof(log_fpair_t));
				p->type = LOG_FMT_CBAK; p->fun = flut[(unsigned)index];

				vector_push_back(f->stack, &p);
			} else
				break;
		} else
			if (!left_ptr)
				left_ptr = itr;
	}

	return f;
}

void log_free_pattern(log_fmt_t* f) {
	if (f) {
		vector_free(f->stack);
		free(f->pool);
		free(f);
	}

	f = NULL;
}

void log_format(log_fmt_t* f, log_msg_t* m) {
	char* beg = m->formatted = malloc(128);
	for (void* it = vector_front(f->stack); it != vector_back(f->stack); it = vector_next(f->stack, it)) {
		log_fpair_t tmp = **(log_fpair_t**)(it);

		assert(tmp.type == LOG_FMT_STRR || tmp.type == LOG_FMT_CBAK);

		if (tmp.type == LOG_FMT_CBAK)
			tmp.fun(m);

		if (tmp.type == LOG_FMT_STRR)
			m->formatted += str_cpy(m->formatted, tmp.pool_ptr);
	}

	m->formatted = beg;
}

void log_set_pattern(const char* p) {
	global_fmt = log_compile_pattern(p);
}

log_fmt_t* log_get_pattern(void) {
	if (!global_fmt)
		global_fmt = log_compile_pattern("%d");

	return global_fmt;
}

void __handle_rule_null(log_msg_t* msg, void* impl) {
	struct __rule_null i = *(struct __rule_null*)impl;
	return;
}

void __handle_rule_stdout(log_msg_t* msg, void* impl) {
	struct __rule_stdout i = *(struct __rule_stdout*)impl;
	fputs(msg->formatted, stdout);
}

void __handle_rule_stderr(log_msg_t* msg, void* impl) {
	struct __rule_stderr i = *(struct __rule_stderr*)impl;
	fputs(msg->formatted, stderr);
}

void __handle_rule_basic(log_msg_t* msg, void* impl) {
	struct __rule_basic i = *(struct __rule_basic*)impl;
	fputs(msg->formatted, i.target);
}

void __handle_rule_capped(log_msg_t* msg, void* impl) {
	struct __rule_capped i = *(struct __rule_capped*)impl;
	assert(false);
}

log_logger_t* log_logger_init(const char* name, log_rule_t* r) {
	if (!global_lmap)
		global_lmap = hashmap_init(8);

	log_logger_t* l = malloc(sizeof(log_logger_t));

	if (!l || !hashmap_set(global_lmap, name, l))
		return NULL;

	/* there should be a better way of doing this.. */
	l->fmt = log_get_pattern(); l->prio = LOG_PRIO_INFO;
	l->name = name; l->rule = r;
	l->counter = 0;

	return l;
}

void log_logger_free(log_logger_t* l) {
	/* waiting for implementing of hashmap_remove */
	/* temporary solution */
	hashmap_set(global_lmap, l->name, NULL);

	if (l)
		free(l);
}

log_logger_t* log_logger_get(const char* name) {
	if (!global_lmap)
		return NULL;

	return (log_logger_t*)hashmap_get(global_lmap, name);
}

void log_free(void) {
	/* iterate over all loggers in the global_lmap */
	/* for (log_logger_t* it = hashmap_begin(global_lmap); */
			 /* it != hashmap_end(global_lmap); */
			 /* it = hashmap_next(it)) { */
		/* if (it) */
			/* free(it); */
	/* } */

	if (global_fmt)
		log_free_pattern(global_fmt);
}
