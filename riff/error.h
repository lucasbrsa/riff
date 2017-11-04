/* @TODO include line func and file info in the log set */
/* @TODO launch breakpoint/stop on error */

/* WHEN PARSING TO THE LOGGER USE THE WRAPPER AND PARSE THE LINE< FUNC BALH BLAH MANULLY */

#ifndef _ERROR_H
#define _ERROR_H

#include <stdarg.h>

#include "generic.h"
#include "log.h"

#define ERROR_MAX_LEN 1024

#define ERROR_FMT "(err) %s"

#define ERROR_PRIO LOG_PRIO_ERROR

#ifndef ERROR_LOG_ONSET
#	define ERROR_LOG_ONSET 1
#endif

typedef struct {
	size_t line, code;
	char msg[ERROR_MAX_LEN];
	const char *file, *func;
} error_t;

/* set the logger to use when an error is set */
void error_logger(log_logger_t* logger);

/* get the current logger */
log_logger_t* error_logger_get(void);

/* form an error_t from all the required data, must be freed */
error_t* error_form_estruct(
		size_t code,
		size_t line,
		const char* file,
		const char* func,
		const char* fmt, ...);

/* add to the end of the error stack */
void error_set_newerror(error_t* error);

/* how many errors on the stack? */
size_t error_get_depth(void);

/* get from error buffer */
error_t* error_get_struct(size_t index);

/* get just the msg from the top of the stack */
/* looses a lot of data when done like this, depreciated */
const char* error_get(void);

/* log the error stack */
void error_log(void);

/* remove error state, free that memory */
void error_clear(void);

#define error_set(fmt, args...) \
	error_set_newerror(error_form_estruct(-1, __LINE__, __FILE__, __func__, fmt, ##args))

#define error_code(code) \
	error_set_newerror(error_form_estruct(code, __LINE__, __FILE__, __func__, NULL))

#define error_set_if(expr, fmt, args...) \
	do { if (expr) { error_set(fmt, ##args); } } while(0)

#define error_code_if(expr, code) \
	do { if (expr) { error_code(code); } } while(0)

#if DEBUG_LEVEL == 1 || defined(ERROR_FORCE_ASSERT)
#	define error_assert(expr) error_set_if(!(expr), "assertion failed: %s", #expr)
#else
#	define error_assert(expression) /**/
#endif

#define ERROR_CODES \
	XX(ERROR_BAD_PARAM, "a function parameter was invalid") \
	XX(ERROR_NULL_PARAM, "a function parameter was NULL") \
	XX(ERROR_FILE, "file IO") \
	XX(ERROR_UNSUPPORTED, "unsupported operation") \
	XX(ERROR_LOGICAL, "logical") \
	XX(ERROR_ALLOCATION, "allocation") \
	XX(ERROR_OS, "operating system") \
	XX(ERROR_OTHER, "other")

enum {
#define XX(a, b) a,
	ERROR_CODES
#undef XX
};

#endif
