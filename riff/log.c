#include <stdarg.h>
#include <assert.h>
#include <malloc.h>
#include <assert.h>

#include "log.h"
#include "os.h"
#include "hashmap.h"
#include "str.h"

#define FORMATTED_BUFFER_SIZE 256
#define ASSUMED_FORMAT_CPAD 64
#define DEFAULT_PATTERN "[%n] %m"

/* globals */
static log_fmt_t* __gfmt = NULL;
static hashmap_t* __gmap = NULL;

/* log rules */
struct __rule_basic { FILE* target; };
struct __rule_capped { FILE* target; size_t lim; };

void __handle_rule_null(log_msg_t* msg, void* impl)
{ return; }

void __handle_rule_stdout(log_msg_t* msg, void* impl)
{ fputs(msg->formatted, stdout); putc('\n', stdout); }

void __handle_rule_stderr(log_msg_t* msg, void* impl)
{ fputs(msg->formatted, stderr); putc('\n', stdout); }

void __handle_rule_basic(log_msg_t* msg, void* impl) {
	FILE* t = (*(struct __rule_basic*)impl).target;
	fputs(msg->formatted, t); putc('\n', t);
}

void __handle_rule_capped(log_msg_t* msg, void* impl) {
	struct __rule_capped i = *(struct __rule_capped*)impl;
	fseek(i.target, 0L, SEEK_END);
	size_t sz = ftell(i.target);

	if (sz < i.lim) {
		fputs(msg->formatted, i.target);
		putc('\n', i.target);
	}
}

void __handle_rule_coloured(log_msg_t* msg, void* impl) {
#ifndef OS_ANSI
	fputs(msg->formatted, stdout);
	putc('\n', stdout);
#else
	/* stole colours from spdlog */
	static char* col_lut[] = {
		/* PANIC */ OS_COLOUR_BOLD OS_COLOUR_ON_RED OS_COLOUR_UNDERLINE,
		/* CRIT */ OS_COLOUR_BOLD OS_COLOUR_RED,
		/* ERROR */ OS_COLOUR_RED,
		/* WARN */ OS_COLOUR_YELLOW,
		/* NOTICE */ OS_COLOUR_CYAN,
		/* INFO */ OS_COLOUR_WHITE,
		/* DEBUG */ OS_COLOUR_WHITE
	};

	assert(msg->priority >= 0 && msg->priority <= LOG_PRIO_DEBUG);
	fputs(col_lut[msg->priority], stdout);
	fputs(msg->formatted, stdout);
	fputs(OS_COLOUR_RESET, stdout);
	putc('\n', stdout);

#endif
}

/* format specifiers */
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
		"panic",
		"crit",
		"error",
		"warn",
		"notice",
		"info",
		"debug"
	};

	assert(msg->priority >= 0 && msg->priority <= (sizeof(lut) / sizeof(char*)));
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

log_fmt_t* log_pattern_compile(const char* fmt) {

	static log_fmod_f flut[26 * 2 + 1] = {
		/* uppercase */
		0, 0, 0, 0, 0, __F, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

		/* lowercase */
		0, 0, 0, 0, 0, __f, 0, 0, __i, 0, 0, __l, __m,
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

void log_pattern_free(log_fmt_t* f) {
	if (f) {
		vector_free(f->stack);
		free(f->pool);
		free(f);
	}

	f = NULL;
}

void log_pattern_apply(log_fmt_t* f, log_msg_t* m) {
	size_t buf_size = FORMATTED_BUFFER_SIZE, diff;
	char* beg = m->formatted;

	for (vector_iterator(f->stack, log_fpair_t*, it)) {
		log_fpair_t tmp = **(log_fpair_t**)(it);

		assert(tmp.type == LOG_FMT_STRR || tmp.type == LOG_FMT_CBAK);

		diff = (size_t)m->formatted - (size_t)beg;
		if (diff + ASSUMED_FORMAT_CPAD >= buf_size) {
			buf_size += MAX(buf_size, ASSUMED_FORMAT_CPAD);
			beg = realloc(beg, buf_size);
			m->formatted = beg + diff;
		}

		if (tmp.type == LOG_FMT_CBAK)
			tmp.fun(m);

		if (tmp.type == LOG_FMT_STRR)
			m->formatted += str_cpy(m->formatted, tmp.pool_ptr);
	}

	m->formatted = beg;
}

void __log_apply_pattern_set(hashmap_bucket_t* b, void* garbage) {
	(*(logger_t*)b->value).fmt = (log_fmt_t*)garbage;
}

void log_pattern_set(const char* p) {
	__gfmt = log_pattern_compile(p);

	hashmap_iterate(__gmap, __log_apply_pattern_set, __gfmt);
}

log_fmt_t* log_pattern_get(void) {
	if (!__gfmt)
		__gfmt = log_pattern_compile(DEFAULT_PATTERN);

	return __gfmt;
}

logger_t* logger_init_custom(
	const char* name,
	log_fmt_t* fmt,
	unsigned prio,
	void (*rule)(log_msg_t* msg, void* impl),
	void* impptr) {

	if (!__gmap)
		__gmap = hashmap_init(8, free);

	logger_t* l = malloc(sizeof(logger_t));

	if (!l || !hashmap_set(__gmap, name, l))
		return NULL;

	l->prio = (prio >= LOG_PRIO_PANIC && prio <= LOG_PRIO_DEBUG)? prio : LOG_PRIO_DEBUG;
	l->fmt = (fmt)? fmt : log_pattern_get();
	l->ruleimpl_ptr = impptr;
	l->rule = rule;
	l->counter = 0;
	l->name = name;

	return l;
}

logger_t* logger_init_null(const char* name)
{ return logger_init_custom(name, NULL, -1, __handle_rule_null, NULL); }

logger_t* logger_init_stdout(const char* name)
{ return logger_init_custom(name, NULL, -1, __handle_rule_stdout, NULL); }

logger_t* logger_init_stderr(const char* name)
{ return logger_init_custom(name, NULL, -1, __handle_rule_stderr, NULL); }

logger_t* logger_init_coloured(const char* name)
{ return logger_init_custom(name, NULL, -1, __handle_rule_coloured, NULL); }

logger_t* logger_init_basic(const char* name, FILE* target) {
	struct __rule_basic* k = malloc(sizeof(struct __rule_basic));
	k->target = target;

	return logger_init_custom(name, NULL, -1, __handle_rule_basic, k);
}

logger_t* logger_init_capped(const char* name, FILE* target, size_t lim) {
	struct __rule_capped* k = malloc(sizeof(struct __rule_capped));
	k->lim = lim;
	k->target = target;

	return logger_init_custom(name, NULL, -1, __handle_rule_capped, k);
}

void logger_free(logger_t* l) {
	if (l->ruleimpl_ptr)
		free(l->ruleimpl_ptr);

	hashmap_remove(__gmap, l->name);
}

logger_t* logger_get(const char* name) {
	if (!__gmap)
		return NULL;

	return (logger_t*)hashmap_get(__gmap, name);
}

void log_free(void) {
	hashmap_free(__gmap);

	if (__gfmt)
		log_pattern_free(__gfmt);
}

void __log(logger_t* l,
		const char* function,
		const char* file,
		size_t line,
		unsigned level,
		const char* fmt, ...) {

	va_list args;
	va_start(args, fmt);

	if (l == NULL || l->prio < level)
		return;

	/* yeash this is shit code */
	char* c = malloc(512);
	vsnprintf(c, 512, fmt, args);
	char* cn = str_strip(c);

	log_msg_t* m = malloc(sizeof(log_msg_t));

	m->message = cn;
	m->lname = l->name;
	m->id = l->counter++;
	m->priority = level;

	m->func = function;
	m->file = file;
	m->line = line;

	time_t t;
	time(&t);
	m->tinfo = localtime(&t);

	m->formatted = malloc(FORMATTED_BUFFER_SIZE);
	log_pattern_apply(l->fmt, m);
	l->rule(m, l->ruleimpl_ptr);

	free(m->formatted);
	free(m);
	free(c);

	va_end(args);
}
