#ifndef _GENERIC_H
#define _GENERIC_H

#define generic_max(a, b) ((a > b) ? a : b)

#define generic_min(a, b) ((a < b) ? a : b)

#define generic_abs(a) ((a > 0)? a : (-1 * (a)))

#define generic_len(a) (sizeof(a) / sizeof(*a))

#define generic_sign(a) ((a > 0) - (a < 0))

#define generic_square(x) ((x) * (x))

#define generic_cube(x) ((x) * (x) * (x))

#define generic_tobool(x) !!(x)

#define generic_floor(x) ((n) - ((n) % 1))

#define generic_ceildiv(x, y)  (((x) + (y) - 1) / (y))

/* help allow for CPP compilation */
#if __STDC_VERSION__ >= 199901L || defined(__cplusplus) || defined(_MSC_VER)
#	define INLINE inline
#else
#	define INLINE /* */
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

#endif