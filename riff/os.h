#ifndef _OS_H
#define _OS_H

#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

/* wrap syscalls */
#define OS_SYSCALL(args) \
	((args) && (system((args)) >= 0))

#define OS_GETCWD /**/

#define OS_RENAME(src, dest) rename(src, dest)

#if _WIN32
#	define OS_MKDIR(dir) _mkdir(dir)
#	define OS_CHDIR(to) _chdir(dir)
#	define OS_RMDIR(dir) _rmdir(dir)
#else
#	define OS_MKDIR(dir) mkdir(dir, S_IRWXU | S_IRWXG | S_IRWXO)
#	define OS_CHDIR(to) chdir(to)
#	define OS_RMDIR(dir) rmdir(dir)
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
