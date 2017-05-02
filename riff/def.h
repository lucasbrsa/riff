#ifndef _DEF_H
#define _DEF_H

#include <float.h>

/* is inline supported? C99? */
#if __STDC_VERSION__ >= 199901L || defined(__cplusplus) || defined(_MSC_VER)
	#define INLINE inline
#else
	#define INLINE
#endif // __STDC_VERSION__...

#ifndef __cplusplus
	#define bool _Bool
	#define true 1
	#define false 0
#endif // __cplusplus

#define DEBUG _DEBUG
#define READONLY const

//logic found here: http://stackoverflow.com/questions/5919996/how-to-detect-reliably-mac-os-x-ios-linux-windows-in-c-preprocessor
#define OS_WINDOWS 0
#define OS_WINDOWS64 0
#define OS_WINDOWS32 0

#define OS_APPLE 0
#define OS_IOS_SIMULATOR 0
#define OS_IOS 0
#define OS_MACOS 0
#define OS_OSX 0
#define OS_APPLE_UNKNOWN 0

#define OS_ANDROID 0
#define OS_LINUX 0
#define OS_UNIX 0
#define OS_POSIX 0
#define OS_UNIX_STYLE 0
#define OS_UNKNOWN 0
#define OS_BSD 0

#if !defined(_WIN32) && (defined(WIN32) || defined(__WIN32) && !defined(__CYGWIN__))
	#define _WIN32
#endif // !defined(_WIN32) && ...

#if !defined(_WIN32) && (defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__)))
	#undef OS_UNIX_STYLE
	#define OS_UNIX_STYLE 1
#endif // !defined(_WIN32) && ..

#if defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
	#include <sys/param.h>
	#if defined(BSD)
		#undef OS_BSD
		#define OS_BSD 1
	#endif // defined(BSD)
#endif // defined(__unix__) ...

#ifdef _WIN32
	#undef OS_WINDOWS
	#define OS_WINDOWS 1

	#ifdef _WIN64
		#undef OS_WINDOWS64
		#define OS_WINDOWS64 1
	#else
		#undef OS_WINDOWS32
		#define OS_WINDOWS32 1
	#endif // _WIN64

#elif __APPLE__
	#undef OS_APPLE
	#define OS_APPLE 1

	//https://opensource.apple.com/source/CarbonHeaders/CarbonHeaders-18.1/TargetConditionals.h
	#include "TargetConditionals.h"

	#if TARGET_IPHONE_SIMULATOR
		#undef OS_IOS_SIMULATOR
		#define OS_IOS_SIMULATOR 1
		#undef OS_IOS
		#define OS_IOS 1
	#elif TARGET_OS_IPHONE
		#undef OS_IOS
		#define OS_IOS 1
	#elif TARGET_OS_MAC
		#undef OS_MACOS
		#define OS_MACOS 1
		#undef OS_OSX
		#define OS_OSX 1
	#else
		#undef OS_APPLE_UNKNOWN
		#define OS_APPLE_UNKNOWN 1
	#endif // TARGET_IPHONE_SIMULATOR
#elif defined(__ANDROID__)
	#undef OS_ANDROID
	#define OS_ANDROID 1
#elif __linux__
	#undef OS_LINUX
	#define OS_LINUX 1
#elif __unix__
	#undef OS_UNIX
	#define OS_UNIX 1
#elif defined(_POSIX_VERSION)
	#undef OS_POSIX
	#define OS_POSIX 1
#else
	#undef OS_UNKNOWN
	#define OS_UNKNOWN 1
#endif // _WIN32 / __APPLE__ ...

#endif // _DEF_H