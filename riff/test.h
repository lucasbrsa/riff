#ifndef _TEST_H
#define _TEST_H

#include <math.h>
#include <string.h>
#include <time.h>
#include <stdio.h>

#define TEST_MSG_LEN 1024
#define TEST_EPSILON 1E-12

static int __test_count, __test_assertion_count, __test_failed_count, __test_state;
static char __test_buf[TEST_MSG_LEN];
static clock_t __test_timer = 0;

typedef void test_f(void);

#define test_resolve() do { \
		clock_t __diff = clock() - __test_timer; \
		printf("\nTime taken: %lds %ldms\n", __diff / 1000, __diff % 1000); \
		printf("Ran: %d tests, with %d assertions, %d of which failed\n", \
				__test_count, __test_assertion_count, __test_failed_count); \
		__test_count = __test_assertion_count = __test_failed_count = \
		__test_state = __test_timer = 0; \
	} while(0)

#define test_run(test) do { \
		if (__test_timer == 0) __test_timer = clock(); \
		__test_state = 0; \
		printf("Running: %s\n", #test); \
		test(); \
		__test_count++; \
		if (__test_state != 0) { \
			printf("%s failed: %s\n", #test, __test_buf); \
		} \
	} while(0)

#define __test_assertion_fail(msg) do { \
		__test_assertion_count++; \
		__test_failed_count++; \
		snprintf(&__test_buf[0], TEST_MSG_LEN, "(%s:%d): %s", __FILE__, __LINE__, msg); \
		__test_state = 1; \
	} while (0)

#define __test_assertion_success() do { \
		__test_assertion_count++; \
		/*printf(".");*/ \
	} while (0)

#define test_assert(expression) do { \
		if (!(expression)) \
			__test_assertion_fail(#expression); \
		else \
			__test_assertion_success(); \
	} while(0)

#define test_fail(message) do { \
		__test_assertion_fail(message); \
	} while(0)

#define test_ieq(a, b) do { \
		int __aexp = (a), __bexp = (b); \
		if (__aexp != __bexp) \
			__test_assertion_fail(##(a == b)); \
		else \
			__test_assertion_success(); \
	} while(0)

#define test_deq(a, b) do { \
		double __aexp = (a), __bexp = (b); \
		if (fabs(__aexp - __bexp) > TEST_EPSILON) \
			__test_assertion_fail(##(a == b)); \
		else \
			__test_assertion_success(); \
	} while(0)

#define test_seq(a, b) do { \
		const char *__aexp = (a), *__bexp = (b); \
		if (__aexp == NULL) __aexp = "(null)"; \
		if (__bexp == NULL) __bexp = "(null)"; \
		if (strcmp(__aexp, __bexp) != 0) \
			__test_assertion_fail(#a " should be equal to " #b); \
		else \
			__test_assertion_success(); \
	} while(0)

#endif
