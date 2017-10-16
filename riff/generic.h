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

#if _WIN64 /* should be a better cross compiler way */
typedef signed long long ssize_t;
#else
typedef signed long ssize_t;
#endif

/* stdbool and stddef replacment */
#if 1
#	ifndef __cplusplus
#		define bool		_Bool
#		define false	0
#		define true		1
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
