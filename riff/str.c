#include "str.h"
#include "def.h"
#include "exception.h"

#include <stdio.h>
#include <string.h>
#include <math.h>

char itoc_s(int i) { return abs(i)%10 + 0x30; }
char itoc_q(int i) { return i + 0x30; }
int ctoi_q(char c) { return c - 0x30; }
int ctoi_s(char c) { return (c >= 0x30 && c <= 0x39) ? c - 0x30 : -1; }

/* goes from the buffer backwards */
char* itostr_q(char* work_space, unsigned short work_len, int i)
{
	ASSERT(work_len > 0); 

	bool neg = false;
	if (neg = (i < 0)) i *= -1;

	char* b = work_space + work_len-- - 1;
	*b = '\0';

	while (i != 0 && work_len > 0)
	{
		*(--b) = itoc_q(i % 10);
		i /= 10;
		work_len--;
	}

	if (neg && work_len > 0) { *(--b) = '-'; }
	return b;
}