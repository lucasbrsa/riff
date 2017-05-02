#ifndef _STR_H
#define _STR_H

/* converts an int to a char 0 = '0' which is not equivalent to (char)0 which would be '\0' */
char itoc_q(int i); /* quick but breaks when 0 > i > 10 */
char itoc_s(int i);
int ctoi_q(char c);
int ctoi_s(char c);

enum
{
	ITOSTR_FORCE_SIGN = 0x1, // if the number is positive prefix with '+'
	ITOSTR_LEFT_PAD = 0x2, // fill padding with zeros instead of spaces
	ITOSTR_USE_SIGN_SPACE = 0x4,
	ITORSTR_USE_TRIPLE_DOT = 0x8,
	ITORSTR_INVERT_JUSTIFY = 0xF
}; /* ItostrFlags */

char* itostr_q(char* work_space, unsigned short work_len, int i);
char* itostr_s(char* work_space, unsigned short write_len, int i, short flags, int width);

#endif