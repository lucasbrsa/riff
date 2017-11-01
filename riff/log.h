/* @TODO syslog openlog calls */
/* @TODO multiple writers */

#ifndef _LOG_H
#define _LOG_H

#include "vector.h"
#include "generic.h"
#include "os.h"

#include <time.h>
#include <stdarg.h>

#ifdef OS_LINUX
#define LOG_SYSLOG
#endif

/* every message contains this data, minimum */
typedef struct {
	/* this is the output buffer of the formatter */
	char* formatted;

	const char *message, *func, *file, *lname;
	struct tm* tinfo;
	size_t line, id, priority;
} log_msg_t;

/* a compiled format is represented by a stack of pairs */
typedef struct {
	char* pool;
	vector_t* stack;
} log_fmt_t;

/* format a message */
void log_fmt(log_fmt_t* f, log_msg_t* m);

/* compile a forat string into a fmt, NULL on failure */
log_fmt_t* log_fmt_compile(const char* pattern);

/* deallocate the compiled format stack */
void log_fmt_free(log_fmt_t* f);

/* list of log priorities */
enum {
	LOG_PRIO_PANIC=0,
	LOG_PRIO_ERROR,
	LOG_PRIO_WARN,
	LOG_PRIO_NOTICE,
	LOG_PRIO_INFO,
	LOG_PRIO_DEBUG
};

/* a log writer is what determines the output of a formatted message */
typedef struct {
	void (*writer)(log_msg_t* msg, void* impl);
	void* writer_ptr; /* cast it to any writer, polymorphism */
} log_writer_t;

/* all the data used by a logger object */
typedef struct {
	const char* name;
	unsigned prio;
	size_t counter;

	log_fmt_t* fmt;
	vector_t* writers;
} log_logger_t;

/* create a new logger instance from a custom writer */
log_logger_t* log_logger_custom(const char* name, log_fmt_t* fmt, log_writer_t* writer);

/* dealloc a logger and remove it from the global table */
void log_logger_free(log_logger_t* l);

/* create a logger */
#define log_logger(name, writer) log_logger_custom(name, NULL, writer)

/* add a new writer to the logger */
void log_logger_add_writer(log_logger_t* logger, log_writer_t* writer);

/* all the default writers */

/* do nothiing with the output */
log_writer_t* log_writer_null(void);

/* write to stdout */
log_writer_t* log_writer_stdout(void);

/* write to stderr */
log_writer_t* log_writer_stderr(void);

/* write to stdout, colour is determined by prio */
log_writer_t* log_writer_stdout_coloured(void);

/* write to stderr, colour is determined by prio */
log_writer_t* log_writer_stderr_coloured(void);

/* write to a file */
log_writer_t* log_writer_file(const char* path);

/* write to a file with a byte limit */
log_writer_t* log_writer_capped(const char* path, size_t lim);

/* rotate to new file when limit is reached */
/* @TODO */

/* linux syslog, will ignore formatting... */
log_writer_t* log_writer_syslog(int option);

/* finally, destroy all globals and unfreed instances */
void log_free(void);

/* execute the logging pipeline, logger -> formatter -> writer */
void log_wrapper(log_logger_t* l,
		const char* function,
		const char* file,
		size_t line,
		unsigned level,
		const char* fmt, ...);

/* the global getters and setters */

/* set the global pattern used by all non-custom fmt loggers */
void log_fmt_set(const char* p);

/* get the global pattern affecing all non-custom fmt loggers */
log_fmt_t* log_fmt_get(void);

/* get a logger or NULL from the global table */
log_logger_t* log_logger_get(const char* name);

/* set the global log level */
void log_priority_set(unsigned p);

/* get the global log level */
unsigned log_priority_get(void);

/* structure queries */

#define log_logger_pattern_get(l) (l->fmt)

#define log_logger_name_get(l) ((l)->name)

#define log_logger_priority_get(l) ((l)->prio)

#define log_logger_writer_get(l) ((l)->writer)

#define log_logger_counter_get(l) ((l)->counter)

#define log_logger_canlog(l, p) ((l)->prio >= (p) && (l)->prio >= log_get_priority())

#define log_logger_priority_set(l, p) do { (l)->prio = (p); } while(0)

#define log_logger_pattern_set(l, p) \
	do { if (l->fmt) { log_fmt_free(l->fmt) } l->fmt = log_compile_pattern(p); } while(0)

/* logger funcs */

#define log_log(logger, prio, fmt, args...) \
	log_wrapper(logger, __func__, __FILE__, __LINE__, prio, fmt, ##args)

#define log_panic(logger, fmt, args...) log_log(logger, LOG_PRIO_PANIC, fmt, ##args)

#define log_error(logger, fmt, args...) log_log(logger, LOG_PRIO_ERROR, fmt, ##args)

#define log_warn(logger, fmt, args...) log_log(logger, LOG_PRIO_WARN, fmt, ##args)

#define log_notice(logger, fmt, args...) log_log(logger, LOG_PRIO_NOTICE, fmt, ##args)

#define log_info(logger, fmt, args...) log_log(logger, LOG_PRIO_INFO, fmt, ##args)

#define log_debug(logger, fmt, args...) log_log(logger, LOG_PRIO_DEBUG, fmt, ##args)

#define log_panic_if(t, logger, fmt, args...) \
	do { if (t) log_log(logger, LOG_PRIO_PANIC, fmt, ##args); } while(0)

#define log_error_if(t, logger, fmt, args...) \
	do { if (t) log_log(logger, LOG_PRIO_ERROR, fmt, ##args); } while(0)

#define log_warn_if(t, logger, fmt, args...) \
	do { if (t) log_log(logger, LOG_PRIO_WARN, fmt, ##args); } while(0)

#define log_notice_if(t, logger, fmt, args...) \
	do { if (t) log_log(logger, LOG_PRIO_NOTICE, fmt, ##args); } while(0)

#define log_info_if(t, logger, fmt, args...) \
	do { if (t) log_log(logger, LOG_PRIO_INFO, fmt, ##args); } while(0)

#define log_debug_if(t, logger, fmt, args...) \
	do { if (t) log_log(logger, LOG_PRIO_DEBUG, fmt, ##args); } while(0)

/* compile time priorities */

#ifndef LOG_LEVEL
#define LOG_LEVEL 5
#endif

#if LOG_LEVEL >= 0
#define LOG_PANIC(logger, fmt, args...) log_log(logger, LOG_PRIO_PANIC, fmt, ##args)
#else
#define LOG_PANIC(logger, fmt, args...) /**/
#endif

#if LOG_LEVEL >= 1
#define LOG_ERROR(logger, fmt, args...) log_log(logger, LOG_PRIO_ERROR, fmt, ##args)
#else
#define LOG_ERROR(logger, fmt, args...) /**/
#endif

#if LOG_LEVEL >= 2
#define LOG_WARN(logger, fmt, args...) log_log(logger, LOG_PRIO_WARN, fmt, ##args)
#else
#define LOG_WARN(logger, fmt, args...) /**/
#endif

#if LOG_LEVEL >= 3
#define LOG_NOTICE(logger, fmt, args...) log_log(logger, LOG_PRIO_NOTICE, fmt, ##args)
#else
#define LOG_NOTICE(logger, fmt, args...) /**/
#endif

#if LOG_LEVEL >= 4
#define LOG_INFO(logger, fmt, args...) log_log(logger, LOG_PRIO_INFO, fmt, ##args)
#else
#define LOG_INFO(logger, fmt, args...) /**/
#endif

#if LOG_LEVEL >= 5
#define LOG_DEBUG(logger, fmt, args...) log_log(logger, LOG_PRIO_DEBUG, fmt, ##args)
#else
#define LOG_DEBUG(logger, fmt, args...) /**/
#endif

/* defaults */

#ifndef LOG_DEFAULT_TIME_FMT
#define LOG_DEFAULT_TIME_FMT "%Y-%m-%d %H:%M:%S"
#endif

#ifndef LOG_DEFAULT_PRIO
#define LOG_DEFAULT_PRIO LOG_PRIO_DEBUG
#endif

#ifndef LOG_DEFAULT_MIN_PRIO
#define LOG_DEFAULT_MIN_PRIO LOG_PRIO_DEBUG
#endif

#ifndef LOG_DEFAULT_FMT_BUFFER_SIZE
#define LOG_DEFAULT_FMT_BUFFER_SIZE 256
#endif

#ifndef LOG_DEFAULT_FMT
#define LOG_DEFAULT_FMT "[%n] %m"
#endif

#ifndef LOG_DEFAULT_FMT_PAD
#define LOG_DEFAULT_FMT_PAD 64
#endif

#endif // _LOG_H
