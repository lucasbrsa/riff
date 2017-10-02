#ifndef _GENERIC_H
#define _GENERIC_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <malloc.h>
#include <string.h>

/* maximum of a and b */
#define MAX(a, b) ((a > b) ? a : b)

/* minimum of a and b */
#define MIN(a, b) ((a < b) ? a : b)

/* absolute value of a */
#define ABS(a) ((a > 0)? a : (-1 * (a)))

/* the sign of a */
#define SIGN(a) ((a > 0) - (a < 0))

/* a^2 */
#define SQUR(a) ((a) * (a))

/* floor of a float/double x */
#define FLOOR(a) ((a) - ((a) % 1))

/* the floor of a / b */
#define CEILD(a, b)  (((a) + (b) - 1) / (b))

/* determine compiler */
#if defined(__clang__)
#	define CMP_CLANG
#elif defined(__ICC) || defined(__INTEL_COMPILER)
#	define CMP_ICC
#elif defined(__GNUC__) || defined(__GNUG__)
#	define CMP_GCC
#elif defined(__HP_cc) || defined(__HP_aCC)
#	define CMP_HP
#elif defined(__IBMC__) || defined(__IBMCPP__)
#	define CMP_IBM
#elif defined(_MSC_VER)
#	define CMP_VS
#elif defined(__PGI)
#	define CMP_PGCC
#elif defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#	define CMP_SOLARIS
#endif

/* determine operating system */
#if !defined(_WIN32) && (defined(WIN32) || defined(__WIN32) && !defined(__CYGWIN__))
#	define _WIN32
#endif

#if !defined(_WIN32) && (defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__)))
#	define OS_UNIX_STYLE
#endif

#if defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
#	include <sys/param.h>
#	if defined(BSD)
#		define OS_BSD
#	endif
#endif

#ifdef _WIN32
#	define OS_WINDOWS
#	ifdef _WIN64
#		define OS_WINDOWS64
#	else
#		define OS_WINDOWS32
#	endif

/* https://opensource.apple.com/source/CarbonHeaders/CarbonHeaders-18.1/TargetConditionals.h */
#elif __APPLE__
#	define OS_APPLE
#	include "TargetConditionals.h"
#	if TARGET_IPHONE_SIMULATOR
#		define OS_IOS_SIMULATOR
#		define OS_IOS
#	elif TARGET_OS_IPHONE
#		define OS_IOS
#	elif TARGET_OS_MAC
#		define OS_MACOS
#		define OS_OSX
#	else
#		define OS_APPLE_UNKNOWN
#	endif
#elif defined(__ANDROID__)
#	define OS_ANDROID
#elif __linux__
#	define OS_LINUX
#elif __unix__
#	define OS_UNIX
#elif defined(_POSIX_VERSION)
#	define OS_POSIX
#else
#	define OS_UNKNOWN
#endif

/* modify some keywords to be more cross platform */

/* help allow for CPP compilation */
#if (__STDC_VERSION__ < 199901L && ! defined(__cplusplus) && ! defined(_MSC_VER))
#	define inline /* */
#endif

#if _WIN64 /* should be a better cross compiler way */
typedef signed long long ssize_t;
#else
typedef signed long ssize_t;
#endif

/* stdbool and stddef replacment */
#if 1
#	ifndef __cplusplus
#		define bool	_Bool
#		define false	0
#		define true	1
#	endif
#	ifndef NULL
#		ifdef __cplusplus
#			define NULL 0
#		else
#			define NULL ((void *)0)
#		endif
#	endif
#endif

#ifdef OS_WINDOWS
#	define __func__ __FUNCTION__
#endif
#ifndef __func__
#	define __func__ "<anonymous>"
#endif

#ifndef va_copy
#	ifdef __va_copy
#		define va_copy __va_copy
#	else
#		define va_copy(d, s) ((d) = (s))
#	endif
#endif

#endif
