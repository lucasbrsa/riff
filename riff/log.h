#ifndef _LOG_H
#define _LOG_H

#include "vector.h"
#include "generic.h"

#include <string.h>
#include <stdio.h>
#include <time.h>

/* all logs are converted into a log_msg_t to be formatted */
/* consider NOT duplicating data from the logger */
typedef struct {
	/* where the final formatted message is written to */
	char* formatted;

	/* extraneous information used by format specifiers */
	const char *message, *func, *file, *lname;
	struct tm* tinfo;
	size_t line, id, priority;
} log_msg_t;

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

/* wrap around a vector of log_fpair_t and store a safe copy of the fmt */
typedef struct {
	char* pool;
	vector_t* stack;
} log_fmt_t;

/* implement the format
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
void log_pattern_apply(log_fmt_t* f, log_msg_t* m);

/* generate a format stack from a format string */
log_fmt_t* log_pattern_compile(const char* fmt);

/* deallocate the format stack */
void log_pattern_free(log_fmt_t* f);

/* set the global pattern used by all loggers */
void log_pattern_set(const char* p);

/* get the global pattern affecing all NEW loggers */
log_fmt_t* log_pattern_get(void);

/* list of log priorities */
/* this is mimicked in the lut */
enum {
	LOG_PRIO_PANIC=0,
	LOG_PRIO_CRIT,
	LOG_PRIO_ERROR,
	LOG_PRIO_WARN,
	LOG_PRIO_NOTICE,
	LOG_PRIO_INFO,
	LOG_PRIO_DEBUG
};

/* object that describes all loggers */
typedef struct {
	const char* name;
	log_fmt_t* fmt;
	void (*rule)(log_msg_t* msg, void* impl);
	void* ruleimpl_ptr; /* cast it to any rule, kinda polymorphism */

	unsigned prio;
	size_t counter;
} logger_t;

/* will be wrapped around w/ macros to define each rule */
logger_t* logger_init_custom(
		const char* name,
		log_fmt_t* fmt,
		unsigned prio,
		void (*rule)(log_msg_t* msg, void* impl),
		void* impptr);

logger_t* logger_init_null(const char* name);
logger_t* logger_init_stdout(const char* name);
logger_t* logger_init_stderr(const char* name);
logger_t* logger_init_coloured(const char* name);
logger_t* logger_init_basic(const char* name, FILE* target);
logger_t* logger_init_capped(const char* name, FILE* target, size_t lim);

/* get a logger or NULL from the global table */
logger_t* logger_get(const char* name);

/* dealloc a logger and remove it from the global table */
void logger_free(logger_t* l);

/* finally, destroy all globals and unfreed instances */
void log_free(void);

/* go through all the stages of logging */
void __log(logger_t* l,
		const char* function,
		const char* file,
		size_t line,
		unsigned level,
		const char* fmt, ...);

/* logger getter/setters */

#define logger_get_pattern(l) \
	(l->fmt)

#define logger_get_name(l) \
	((l)->name)

#define logger_get_priority(l, p) \
	((l)->prio)

#define logger_canlog(l, p) \
	((l)->prio <= (p))

#define logger_set_priority(l, p) \
	do { (l)->prio = (p); } while(0)

#define logger_set_pattern(l, p) \
	do { l->fmt = log_compile_pattern(p); } while(0)

/* logger generators for rules*/
#define logp(logger_name, prio, fmt, args...) \
	__log(logger_name, __func__, __FILE__, __LINE__, prio, fmt, ##args)

#define logp_panic(logger_val, fmt, args...) \
	logp(logger_val, LOG_PRIO_PANIC, fmt, ##args)

#define logp_crit(logger_val, fmt, args...) \
	logp(logger_val, LOG_PRIO_CRIT, fmt, ##args)

#define logp_error(logger_val, fmt, args...) \
	logp(logger_val, LOG_PRIO_ERROR, fmt, ##args)

#define logp_warn(logger_val, fmt, args...) \
	logp(logger_val, LOG_PRIO_WARN, fmt, ##args)

#define logp_notice(logger_val, fmt, args...) \
	logp(logger_val, LOG_PRIO_NOTICE, fmt, ##args)

#define logp_info(logger_val, fmt, args...) \
	logp(logger_val, LOG_PRIO_INFO, fmt, ##args)

#define logp_debug(logger_val, fmt, args...) \
	logp(logger_val, LOG_PRIO_DEBUG, fmt, ##args)

#define logp_panic_if(logger_val, expression, fmt, args...) \
	do { if (expression) logp_panic(logger_val, fmt, ##args); } while(0)

#define logp_crit_if(logger_val, expression, fmt, args...) \
	do { if (expression) logp_crit(logger_val, fmt, ##args); } while(0)

#define logp_error_if(logger_val, expression, fmt, args...) \
	do { if (expression) logp_error(logger_val, fmt, ##args); } while(0)

#define logp_warn_if(logger_val, expression, fmt, args...) \
	do { if (expression) logp_warn(logger_val, fmt, ##args); } while(0)

#define logp_notice_if(logger_val, expression, fmt, args...) \
	do { if (expression) logp_notice(logger_val, fmt, ##args); } while(0)

#define logp_info_if(logger_val, expression, fmt, args...) \
	do { if (expression) logp_info(logger_val, fmt, ##args); } while(0)

#define logp_debug_if(logger_val, expression, fmt, args...) \
	do { if (expression) logp_debug(logger_val, fmt, ##args); } while(0)

/* would like to call 'log' but conflicts with <math.h> log */
#define logl(logger_name, prio, fmt, args...) \
	__log(logger_get(logger_name), __func__, __FILE__, __LINE__, prio, fmt, ##args)

#define log_panic(logger_name, fmt, args...) \
	logl(logger_name, LOG_PRIO_PANIC, fmt, ##args)

#define log_crit(logger_name, fmt, args...) \
	logl(logger_name, LOG_PRIO_CRIT, fmt, ##args)

#define log_error(logger_name, fmt, args...) \
	logl(logger_name, LOG_PRIO_ERROR, fmt, ##args)

#define log_warn(logger_name, fmt, args...) \
	logl(logger_name, LOG_PRIO_WARN, fmt, ##args)

#define log_notice(logger_name, fmt, args...) \
	logl(logger_name, LOG_PRIO_NOTICE, fmt, ##args)

#define log_info(logger_name, fmt, args...) \
	logl(logger_name, LOG_PRIO_INFO, fmt, ##args)

#define log_debug(logger_name, fmt, args...) \
	logl(logger_name, LOG_PRIO_DEBUG, fmt, ##args)

#define log_panic_if(logger_name, expression, fmt, args...) \
	do { if (expression) log_panic(logger_name, fmt, ##args); } while(0)

#define log_crit_if(logger_name, expression, fmt, args...) \
	do { if (expression) log_crit(logger_name, fmt, ##args); } while(0)

#define log_error_if(logger_name, expression, fmt, args...) \
	do { if (expression) log_error(logger_name, fmt, ##args); } while(0)

#define log_warn_if(logger_name, expression, fmt, args...) \
	do { if (expression) log_warn(logger_name, fmt, ##args); } while(0)

#define log_notice_if(logger_name, expression, fmt, args...) \
	do { if (expression) log_notice(logger_name, fmt, ##args); } while(0)

#define log_info_if(logger_name, expression, fmt, args...) \
	do { if (expression) log_info(logger_name, fmt, ##args); } while(0)

#define log_debug_if(logger_name, expression, fmt, args...) \
	do { if (expression) log_debug(logger_name, fmt, ##args); } while(0)

#endif // _LOG_H
