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

#endif
