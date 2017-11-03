#ifndef _ERROR_H
#define _ERROR_H

#include <stdarg.h>

#include "generic.h"
#include "log.h"

#define ERROR_MAX_LEN 1024
#define __ERROR_XMENUM \
	XX(ERROR_BAD_PARAM, "bad parameter") \
	XX(ERROR_NO_MEM, "no memory") \
	XX(ERROR_FILE, "file") \
	XX(ERROR_UNSUPPORTED, "unsupported operation") \
	XX(ERROR_LOGICAL, "logical") \
	XX(ERROR_NOT_ALLOCED, "not allocated") \
	XX(ERROR_OS, "not allocated") \
	XX(ERROR_OTHER, "other")

/* set the logger to use when an error is set */
void error_logger(log_logger_t* logger);

/* get the current logger */
log_logger_t* error_logger_get(void);

/* what prio to use, thus errors may become warnings etc.. */
void error_set_prio(unsigned log_prio);

/* set the current error value */
void error_set(const char* fmt, ...);

/* set the current value based on lut */
void error_code(size_t code);

/* get from error buffer */
const char* error_get(void);

/* get but give "no error" is none or clear */
const char* error_gets(void);

/* remove error state, free that memory */
void error_clear(void);

enum {
#define XX(a, b) a,
	__ERROR_XMENUM
#undef XX
};

#define error_set_if(expression, fmt, args...) \
	do { if (expression) { error_set(fmt, ##args); } } while(0)

#define __error_assert(expression) \
	do { if(!(expression)) { \
		error_set("assertion failed (%s:%d): %s", __FILE__, __LINE__, #expression); \
	} } while(0)

#if DEBUG_LEVEL == 1 || defined(ERROR_FORCE_ASSERT)
#	define error_assert(expression) __error_assert(expression)
#else
#	define error_assert(expression) /**/
#endif

#endif
