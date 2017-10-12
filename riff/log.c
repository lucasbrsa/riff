#include <assert.h>
#include <malloc.h>
#include <assert.h>

#include "hashmap.h"
#include "log.h"
#include "str.h"

#define FORMATTED_BUFFER_SIZE 256

/* globals */
static log_fmt_t* global_fmt = NULL;
static hashmap_t* global_lmap = NULL;

void __d(log_msg_t* msg) {
	static log_fmt_t* f = NULL;

	if (!f)
		f = log_compile_pattern("[%n] %m");

	log_format(f, msg);
	msg->formatted += strlen(msg->formatted);
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

				vector_push_back(f->stack, p);
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

				vector_push_back(f->stack, p);
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
	char* beg = m->formatted;

	for (vector_iterator(f->stack, log_fpair_t*, it)) {
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
	return;
}

void __handle_rule_stdout(log_msg_t* msg, void* impl) {
	puts(msg->formatted);
}

void __handle_rule_stderr(log_msg_t* msg, void* impl) {
	fputs(msg->formatted, stderr);
}

void __handle_rule_basic(log_msg_t* msg, void* impl) {
	struct __rule_basic i = *(struct __rule_basic*)impl;
	fputs(msg->formatted, i.target);
}

void __handle_rule_capped(log_msg_t* msg, void* impl) {
	struct __rule_capped i = *(struct __rule_capped*)impl;
	fseek(i.target, 0L, SEEK_END);
	size_t sz = ftell(i.target);

	if (sz < i.lim)
		fputs(msg->formatted, i.target);
}

log_logger_t* log_logger_init(const char* name, log_rule_t* r) {
	if (!global_lmap)
		global_lmap = hashmap_init(8, free);

	log_logger_t* l = malloc(sizeof(log_logger_t));

	if (!l || !hashmap_set(global_lmap, name, l))
		return NULL;

	l->fmt = log_get_pattern();
	l->prio = LOG_PRIO_INFO;
	l->counter = 0;
	l->name = name;
	l->rule = r;

	return l;
}

void log_logger_free(log_logger_t* l) {
	hashmap_remove(global_lmap, l->name);
}

log_logger_t* log_logger_get(const char* name) {
	if (!global_lmap)
		return NULL;

	return (log_logger_t*)hashmap_get(global_lmap, name);
}

void log_free(void) {
	hashmap_free(global_lmap);

	if (global_fmt)
		log_free_pattern(global_fmt);
}

void log_log(log_logger_t* l, const char* c) {
	log_msg_t* m = malloc(sizeof(log_msg_t));

	m->message = c;
	m->lname = l->name;
	//m->id = hashmap_hash(global_lmap, l->name) + l->counter++;
	m->priority = l->prio;

	/* tmp */
	m->func = "main";
	m->file = "main.c";
	m->line = 14;

	time_t t;
	time(&t);
	m->tinfo = localtime(&t);

	m->formatted = malloc(FORMATTED_BUFFER_SIZE);
	log_format(l->fmt, m);
	l->rule->rule(m, l->rule->ruleimpl_ptr);

	free(m->formatted);
	free(m);
}
