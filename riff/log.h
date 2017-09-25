#ifndef _LOG_H
#define _LOG_H

#include "vector.h"
#include "generic.h"

#include <string.h>
#include <stdio.h>

typedef struct {
	char* out;
	const char* in;
	// ...
} log_msg_t;

typedef void (*log_fmt_mod_fun)(log_msg_t* msg);

typedef struct {
	enum {
		LOG_FMT_STRR=0,
		LOG_FMT_CBAK=1,
	} type;

	union {
		char* pool_ptr;
		log_fmt_mod_fun fun;
	};
} log_fmt_pair_t;

typedef struct {
	char* pool;
	vector_t* stack;
} log_fmt_t;

typedef enum {
	LOG_PRIORITY_TRACE = 0,
	LOG_PRIORITY_DEBUG,
	LOG_PRIORITY_INFO,
	LOG_PRIORITY_WARN,
	LOG_PRIORITY_ERROR,
	LOG_PRIORITY_CRIT
} log_priority_t;

static log_fmt_t global_compiled_format;

/* a state machine object, as generated statically or individually */
typedef struct {
	const char* name;
	log_fmt_t fmt;
	log_priority_t level;
	int counter;
} log_logger_t;

log_fmt_t* log_compile_pattern(const char* fmt);
void log_free_pattern(log_fmt_t* f);

void log_test(log_fmt_t* f, log_msg_t* m);

//log_logger_t* log_logger(const char* name, FILE* out, ...); // fill in this shit later

//#define log_logger_stdout(name) \
//	log_logger(name, stdout)
//...
//log_logger_t* log_logger_stdout(const char* name);
//log_logger_t* log_logger_stderr(const char* name);
//log_logger_t* log_logger_stdout_colour(const char* name);
//log_logger_t* log_logger_stderr_colour(const char* name);
//log_logger_t* log_logger_basic(const char* name, FILE* out);

/* logging functions */
//void log(log_logger_t* l, /* level and literally all the other info required (__LINE__ and shit to construct a log_msg_t)*/ const char* fmt, ...)

//macros that wrap around log for:
/*
trace
debug
info
warn
error
crit

trace_if
debug_if
info_if
warn_if
error_if
crit_if
*/

bool log_canlog(log_logger_t* l);

void log_set_level(log_logger_t* l, log_priority_t nv);
log_priority_t log_get_level(log_logger_t* l);

log_fmt_t* log_get_fmt(log_logger_t* l);
void log_set_fmt(log_logger_t* l, log_fmt_t* f);

// !!!!!!! ----->>>>>>>> set / get the global default logger pattern

#endif // _LOG_H
