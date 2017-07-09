#include "base64.h"

#include <assert.h>

/* the base64 LUT */
static const char base64_lut[257] =
	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"
	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"
	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"
	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";

//char* base64_encode(char* dest, const char* src) {
//
//}

//char* base64_decode(char* dest, const char* src) {
//	assert("b64 src must be MIME complaint", (strlen(src) % 3));
//}