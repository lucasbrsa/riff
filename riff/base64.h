#ifndef _BASE64_H
#define _BASE64_H

/* encode an ANSI ASCII string into MIME complaint base64 */
char* base64_encode(char* dest, const char* src);

/* decode a MIME complaint base64 string to an ANSI ASCII string*/
char* base64_decode(char* dest, const char* src);

#endif