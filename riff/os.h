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

#ifdef linux
#	define OS_LINUX
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


#ifndef OS_WINDOWS
#	define OS_ANSI
#	define OS_COLOUR_RESET "\033[00m"
#	define OS_COLOUR_BOLD "\033[1m"
#	define OS_COLOUR_DARK "\033[2m"
#	define OS_COLOUR_UNDERLINE "\033[4m"
#	define OS_COLOUR_BLINK "\033[5m"
#	define OS_COLOUR_REVERSE "\033[7m"
#	define OS_COLOUR_CONCEALED "\033[8m"

#	define OS_COLOUR_GREY "\033[30m"
#	define OS_COLOUR_RED "\033[31m"
#	define OS_COLOUR_GREEN "\033[32m"
#	define OS_COLOUR_YELLOW "\033[33m"
#	define OS_COLOUR_BLUE "\033[34m"
#	define OS_COLOUR_MAGENTA "\033[35m"
#	define OS_COLOUR_CYAN "\033[36m"
#	define OS_COLOUR_WHITE "\033[37m"

#	define OS_COLOUR_ON_GREY "\033[40m"
#	define OS_COLOUR_ON_RED "\033[41m"
#	define OS_COLOUR_ON_GREEN "\033[42m"
#	define OS_COLOUR_ON_YELLOW "\033[43m"
#	define OS_COLOUR_ON_BLUE "\033[44m"
#	define OS_COLOUR_ON_MAGENTA "\033[45m"
#	define OS_COLOUR_ON_CYAN "\033[46m"
#	define OS_COLOUR_ON_WHITE "\033[47m"
#endif

#endif
