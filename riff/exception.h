#ifndef _EXCEPTION_H
#define _EXCEPTION_H

/* holds information about the macro caller */
typedef struct
{
	int line;
	const char* file;
	const char* func;

} CallingContext;

#define __CURRENT_CALLING_CONTEXT (CallingContext) { __LINE__, __FILE__, __func__ }

/* this logic is run, on an assertion failure */
void __assertion_failure(CallingContext cc, const char* expr);

#ifdef NDEBUG
#define ASSRT(expression) (void(0))
#else
#define ASSERT(expression) ( (expression)?((void)0) : __assertion_failure(__CURRENT_CALLING_CONTEXT, #expression) )
#endif

#endif