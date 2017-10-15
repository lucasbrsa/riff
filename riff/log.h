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

/* set the global pattern used by all NEW loggers */
void log_pattern_set(const char* p);

/* get the global pattern affecing all NEW loggers */
log_fmt_t* log_pattern_get(void);

/* the final stage after implementing formatting: how to output the message */
typedef struct {
	void (*rule)(log_msg_t* msg, void* impl);
	void* ruleimpl_ptr; /* cast it to any rule, kinda polymorphism */
} log_rule_t;

/* list of different log rules */
/* empty structs redundant, will be used once rule stuff is generalized with macros */
struct __rule_basic { FILE* target; };
struct __rule_capped { FILE* target; size_t lim; };

/* handlers for each of those rules */
void __handle_rule_null(log_msg_t* msg, void* impl);
void __handle_rule_stdout(log_msg_t* msg, void* impl);
void __handle_rule_stderr(log_msg_t* msg, void* impl);
void __handle_rule_basic(log_msg_t* msg, void* impl);
void __handle_rule_capped(log_msg_t* msg, void* impl);

/* list of log priorities */
/* this is mimicked in the lut */
enum {
	LOG_PRIO_PANIC=0,
	LOG_PRIO_ALERT,
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
	log_rule_t* rule;

	unsigned prio;
	size_t counter;
} logger_t;

/* will be wrapped around w/ macros to define each rule */
logger_t* logger_init(const char* name, log_rule_t* r);

/* get a logger or NULL from the global table */
logger_t* logger_get(const char* name);

/* dealloc a logger and remove it from the global table */
void logger_free(logger_t* l);

/* finally, destroy all globals and unfreed instances */
void log_free(void);

/* go through all the stages of logging */
void __llog(logger_t* l, const char* c, const char* function, const char* file, size_t line);

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
/* ... */

/* macros that wrap around log */
#define llog(logger, message) __llog(logger, message, __func__, __FILE__, __LINE__);

/* ie.
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

#endif // _LOG_H
