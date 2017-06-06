#ifdef _LOGGING_H
#	ifndef _LOGDEF_H
#	define _LOGDEF_H
#	ifndef _MSC_VER
#		define __FUNCTION__ "<anonymous>"
#	endif
#	ifndef _WIN32
#		define LOG_DIRCHR '/'
#	else
#		define LOG_DIRCHR '\\'
#	endif
#	define LOG_FILENAME (strrchr(__FILE__, LOG_DIRCHR) ? strrchr(__FILE__, LOG_DIRCHR) + 1 : __FILE__)
#	define log(cat, prio, fmt, ...) log_message(cat, prio, __LINE__, LOG_FILENAME, __FUNCTION__, fmt, __VA_ARGS__)
#	define logf(fmt, ...) log(LOG_CAT_APPLICATION, LOG_DEFAULT_PRIO, fmt, __VA_ARGS__)
#	define log_trace(cat, fmt, ...) log(cat, LOG_PRIO_TRACE, fmt, __VA_ARGS__)
#	define log_debug(cat, fmt, ...) log(cat, LOG_PRIO_DEBUG, fmt, __VA_ARGS__)
#	define log_info(cat, fmt, ...) log(cat, LOG_PRIO_INFO, fmt, __VA_ARGS__)
#	define log_warn(cat, fmt, ...) log(cat, LOG_PRIO_WARN, fmt, __VA_ARGS__)
#	define log_error(cat, fmt, ...) log(cat, LOG_PRIO_ERROR, fmt, __VA_ARGS__)
#	define log_fatal(cat, fmt, ...) log(cat, LOG_PRIO_FATAL, fmt, __VA_ARGS__)
#	if !defined(LOG_MCAT) && !defined(LOG_M)
#		define LCAPP LOG_CAT_APPLICATION
#		define LCASRT LOG_CAT_ASSERTION
#		define LCTEST LOG_CAT_TEST
#		define LCERR LOG_CAT_ERROR
#		define LCSYS LOG_CAT_SYS
#		define LCREND LOG_CAT_RENDER
#		define LCCUS(n) ( (n > LOG_CAT_RESERVED7)? LOG_CAT_CUSTOM##n : NULL )
#		define LCRES(n) ( (n >= 0 && n <= (LOG_CAT_RESERVED7-LOG_CAT_RESERVED0))? LOG_CAT_RESERVED##n : NULL )
#	endif
#	if !defined(LOG_MPRIO) && !defined(LOG_M)
#		define LPTRCE LOG_PRIO_TRACE
#		define LPDBG LOG_PRIO_DEBUG
#		define LPINFO LOG_PRIO_INFO
#		define LPWARN LOG_PRIO_WARN
#		define LPERR LOG_PRIO_ERROR
#		define LPFTL LOG_PRIO_FATAL
#	endif
#endif
#else
#	pragma warning("Warning: logdef.h not used, logging.h must be included first")
#endif