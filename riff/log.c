#include <assert.h>
#include <string.h>
#include <malloc.h>
#include <assert.h>

#include "error.h"
#include "log.h"
#include "hashmap.h"
#include "str.h"

#ifdef LOG_SYSLOG
#include <syslog.h>
#endif

/* globals */
static log_fmt_t* __gfmt = NULL;
static hashmap_t* __gmap = NULL;
static unsigned __gprio = LOG_PRIO_DEBUG;

/* a format speicifer call */
typedef void (*log_fmod_f)(log_msg_t* msg);

/* an instance on the fmt stack is one of two type */
typedef struct {
	enum {
		LOG_FMT_STRR=0,
		LOG_FMT_CBAK=1,
	} type;

	union {
		char* pool_ptr;
		log_fmod_f fun;
	};
} log_fpair_t;

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
		"error",
		"warn",
		"notice",
		"info",
		"debug"
	};

	if (!(msg->priority <= (sizeof(lut) / sizeof(char*)))) {
		error_set("used %p in log fmt, but could not convert prio to string");
		return;
	}

	msg->formatted += str_cpy(msg->formatted, lut[msg->priority]);
}

void __s(log_msg_t* msg) {
	static const char* lut = "LDIWEC";


	if (!(msg->priority < sizeof("LDIWEC"))) {
		error_set("used %s in log fmt, but could not convert prio to string");
		return;
	}

	*msg->formatted++ = *(lut + msg->priority);
}

void __t(log_msg_t* msg) {
	static char stbuf[26];

	/* replace with global time format */
	strftime(stbuf, 26, LOG_DEFAULT_TIME_FMT, msg->tinfo);
	msg->formatted += str_cpy(msg->formatted, stbuf);
}

void __percent (log_msg_t* msg) {
	*msg->formatted++ = '%';
}

/* format specifiers
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
log_fmt_t* log_fmt_compile(const char* pattern) {

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
	f->pool = str_dup(pattern);

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

void log_fmt_free(log_fmt_t* f) {
	if (f) {
		vector_free(f->stack);
		free(f->pool);
		free(f);
	}

	f = NULL;
}

void log_fmt(log_fmt_t* f, log_msg_t* m) {
	size_t buf_size = LOG_DEFAULT_FMT_BUFFER_SIZE, diff;
	char* beg = m->formatted;

	for (vector_iterator(f->stack, log_fpair_t*, it)) {
		log_fpair_t tmp = **(log_fpair_t**)(it);

		if (!(tmp.type == LOG_FMT_STRR || tmp.type == LOG_FMT_CBAK)) {
			error_set("when parsing log fmt the fmt stack become corrupted");
			continue;
		}

		diff = (size_t)m->formatted - (size_t)beg;
		if (diff + LOG_DEFAULT_FMT_PAD >= buf_size) {
			buf_size += MAX(buf_size, LOG_DEFAULT_FMT_PAD);
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

void __log_fmt_set(hashmap_bucket_t* b, void* garbage) {
	(*(log_logger_t*)b->value).fmt = (log_fmt_t*)garbage;
}

void log_fmt_set(const char* p) {
	__gfmt = log_fmt_compile(p);

	hashmap_iterate(__gmap, __log_fmt_set, __gfmt);
}

log_fmt_t* log_fmt_get(void) {
	if (!__gfmt)
		__gfmt = log_fmt_compile(LOG_DEFAULT_FMT);

	return __gfmt;
}

void log_set_priority(unsigned p) {
	if (p >= LOG_PRIO_PANIC && p <= LOG_PRIO_DEBUG)
		__gprio = p;
}

unsigned log_get_priority(void) {
	return __gprio;
}

/* log rules */
struct __writer_data_basic { FILE* target; };
struct __writer_data_capped { FILE* target; size_t lim; };

void __writer_null(log_msg_t* msg, void* impl)
{ return; }

void __writer_stdout(log_msg_t* msg, void* impl)
{ fputs(msg->formatted, stdout); putc('\n', stdout); }

void __writer_stderr(log_msg_t* msg, void* impl)
{ fputs(msg->formatted, stderr); putc('\n', stdout); }

void __writer_basic(log_msg_t* msg, void* impl) {
	FILE* t = (*(struct __writer_data_basic*)impl).target;
	fputs(msg->formatted, t); putc('\n', t);
}

void __writer_capped(log_msg_t* msg, void* impl) {
	struct __writer_data_capped i = *(struct __writer_data_capped*)impl;
	fseek(i.target, 0L, SEEK_END);
	size_t sz = ftell(i.target);

	if (sz < i.lim) {
		fputs(msg->formatted, i.target);
		putc('\n', i.target);
	}
}

void __writer_syslog(log_msg_t* msg, void* impl) {
#ifdef LOG_SYSLOG

	static int syslog_prio_lut[] = {
		LOG_CRIT,
		LOG_ERR,
		LOG_WARNING,
		LOG_NOTICE,
		LOG_INFO,
		LOG_DEBUG
	};

	assert(msg->priority >= LOG_PRIO_PANIC && msg->priority <= LOG_PRIO_DEBUG);
	syslog(syslog_prio_lut[msg->priority], "%s", msg->message);

#endif
}

void ____writer_coloured(log_msg_t* msg, void* impl, FILE* target) {
#ifndef OS_ANSI

	fputs(msg->formatted, stdout);
	putc('\n', stdout);

#else
	/* stole colours from spdlog */
	static char* col_lut[] = {
		/* PANIC */ OS_COLOUR_BOLD OS_COLOUR_ON_RED OS_COLOUR_UNDERLINE,
		/* ERROR */ OS_COLOUR_BOLD OS_COLOUR_RED,
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

void __writer_stdout_coloured(log_msg_t* msg, void* impl) {
	____writer_coloured(msg, impl, stdout);
}

void __writer_stderr_coloured(log_msg_t* msg, void* impl) {
	____writer_coloured(msg, impl, stderr);
}

void __writer_free(void* ptr) {
	log_writer_t* w = (log_writer_t*)ptr;

	if (w->writer_ptr)
		free(w->writer_ptr);

	free(ptr);
}

log_logger_t* log_logger_custom(const char* name, log_fmt_t* fmt, log_writer_t* writer) {
	if (!__gmap)
		__gmap = hashmap_init(8, free);

	log_logger_t* l = malloc(sizeof(log_logger_t));

	if (!l || !hashmap_set(__gmap, name, l))
		return NULL;

	l->name = name;
	l->prio = LOG_DEFAULT_PRIO;
	l->counter = 0;
	l->fmt = (fmt)? fmt : log_fmt_get();
	l->writers = vector_init(1, sizeof(log_writer_t*), __writer_free);

	log_logger_add_writer(l, writer);

	return l;
}

void log_logger_add_writer(log_logger_t* logger, log_writer_t* writer) {
	vector_push_back(logger->writers, writer);
}

log_writer_t* log_writer_null(void) {
	log_writer_t* w = malloc(sizeof(log_writer_t));
	w->writer = __writer_null;
	w->writer_ptr = NULL;
	return w;
}

log_writer_t* log_writer_stdout(void) {
	log_writer_t* w = malloc(sizeof(log_writer_t));
	w->writer = __writer_stdout;
	w->writer_ptr = NULL;
	return w;
}

log_writer_t* log_writer_stderr(void) {
	log_writer_t* w = malloc(sizeof(log_writer_t));
	w->writer = __writer_stderr;
	w->writer_ptr = NULL;
	return w;
}

log_writer_t* log_writer_stderr_coloured(void) {
	log_writer_t* w = malloc(sizeof(log_writer_t));
	w->writer = __writer_stderr_coloured;
	w->writer_ptr = NULL;
	return w;
}

log_writer_t* log_writer_stdout_coloured(void){
	log_writer_t* w = malloc(sizeof(log_writer_t));
	w->writer = __writer_stdout_coloured;
	w->writer_ptr = NULL;
	return w;
}

log_writer_t* log_writer_syslog(int option){
	log_writer_t* w = malloc(sizeof(log_writer_t));
	w->writer = __writer_syslog;
	w->writer_ptr = NULL;
	return w;
}

log_writer_t* log_writer_file(const char* path) {
	FILE* targ = fopen(path, "a");

	assert(targ != NULL);

	struct __writer_data_basic* k = malloc(sizeof(struct __writer_data_basic));
	k->target = targ;

	log_writer_t* w = malloc(sizeof(log_writer_t));
	w->writer = __writer_basic;
	w->writer_ptr = k;

	return w;
}

log_writer_t* log_writer_capped(const char* path, size_t lim) {
	FILE* targ = fopen(path, "a");

	if (targ == NULL) {
		error_code(ERROR_FILE);
		return NULL;
	}

	struct __writer_data_capped* k = malloc(sizeof(struct __writer_data_capped));
	k->target = targ;
	k->lim = lim;

	log_writer_t* w = malloc(sizeof(log_writer_t));
	w->writer = __writer_capped;
	w->writer_ptr = k;

	return w;
}

void log_logger_free(log_logger_t* l) {
	vector_free(l->writers);
	hashmap_remove(__gmap, l->name);
}

log_logger_t* log_logger_get(const char* name) {
	if (!__gmap)
		return NULL;

	return (log_logger_t*)hashmap_get(__gmap, name);
}

void __writer_free_from_bucket(hashmap_bucket_t* bucket, void* garbage) {
	vector_free(((log_logger_t*)bucket->value)->writers);
}

void log_free(void) {
	hashmap_iterate(__gmap, __writer_free_from_bucket, NULL);
	hashmap_free(__gmap);

	if (__gfmt)
		log_fmt_free(__gfmt);

	__gmap = NULL;
	__gfmt = NULL;
}

void log_wrapper(log_logger_t* l,
		const char* function,
		const char* file,
		size_t line,
		unsigned level,
		const char* fmt, ...) {

	va_list args;
	va_start(args, fmt);

	if (l == NULL || !log_logger_canlog(l, level))
		return;

	char* c = str_vsprintf(fmt, args);
	log_msg_t* m = malloc(sizeof(log_msg_t));

	m->message = str_strip(c);
	m->lname = l->name;
	m->id = l->counter++;
	m->priority = level;

	m->func = function;
	m->file = file;
	m->line = line;

	time_t t;
	time(&t);
	m->tinfo = localtime(&t);

	m->formatted = malloc(LOG_DEFAULT_FMT_BUFFER_SIZE);
	log_fmt(l->fmt, m);

	for (vector_iterator(l->writers, log_writer_t*, i))
		(*i)->writer(m, (*i)->writer_ptr);

	free(m->formatted);
	free(m);
	free(c);

	va_end(args);
}
