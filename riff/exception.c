#include "exception.h"

#include <stdio.h>
#include <stdlib.h>

/* this logic is run, on an assertion failure */
void __assertion_failure(CallingContext cc, const char* expr)
{
	fputs("Debug assertion failed!\n\n", stderr);
	fprintf(stderr, "File: %s\n", cc.file);
	fprintf(stderr, "Func: %s\n", cc.func);
	fprintf(stderr, "Line: %i\n\n", cc.line);
	
	fprintf(stderr, "Expression: (%s) failed\n", expr);

	getc(stdin);
	exit(1);
}