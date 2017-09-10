/*
#include "vector.h"
#include "fmt.h"
#include "str.h"

#include <string.h>

typedef struct {
	float intern;
	char* msg;
} continuous_data_t;

size_t __fmt_time(char* out, void* itrpt_strct);
size_t __fmt_date(char* out, void* itrpt_strct);
size_t __fmt_float(char* out, void* itrpt_strct);
size_t __fmt_msg(char* out, void* itrpt_strct);

static fmt_symfunc_f flut[26 * 2] = {
#define t __fmt_time
#define d __fmt_date
#define f __fmt_float
#define m __fmt_msg
	   0,0,0,d,0,f,0,0,0,0,0,0,m,0,0,0,0,0,0,t,0,0,0,0,0,0,
	   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
#undef t
#undef d
#undef f
#undef m
};

#define fmt_str "date: %d, time: %t, message: %m, internal float: %f more txt.."
// #define fmt_str "%%%d%k%^date: %d, time: %t, message: %m, internal float: %f"

int main() {
	char* buf_src = str_dup(fmt_str);
	continuous_data_t dt = { .intern = 3.14159,.msg = "sample message!" };
	fmt_t* f = fmt_compile(buf_src, flut);
	fmt_print(f, stdout, (void*)&dt, 512);
	fmt_free(f);
	free(buf_src);

	return getchar();
}

size_t __fmt_time(char* out, void* itrpt_strct) {
	memcpy(out, "6:33pm", sizeof("6:33pm"));
	return sizeof("6:33pm");
}
size_t __fmt_date(char* out, void* itrpt_strct) {
	memcpy(out, "05/09/17", sizeof("05/09/17"));
	return sizeof("05/09/17");
}
size_t __fmt_float(char* out, void* itrpt_strct) {
	memcpy(out, "4.2347", sizeof("4.2347"));
	return sizeof("4.2347");
}
size_t __fmt_msg(char* out, void* itrpt_strct) {
	char* k = (*(continuous_data_t*)itrpt_strct).msg;
	size_t len = strlen(k);
	memcpy(out, k, len);
	return len;
}

*/

/* oh L A R I S S A, don't hurt me ! */
#include <stdio.h>
#include "hashmap.h"

int main(int argc, char ** argv)
{
	hashmap_t* h = hashmap_create(10);

	hashmap_set(h, "age", "15");
	hashmap_set(h, "sex", "none");
	hashmap_set(h, "gf", "jenny");
	hashmap_set(h, "gf", "celina");
	hashmap_set(h, "gf", "priya");
	hashmap_set(h, "gf", "isabelle");

	for (hashmap_bucket_t* it = hashmap_begin(h); it != hashmap_end(h); hashmap_next(it))
		if (it->key && it->value)
			printf("%p\t%s\t%s\n", it, it->key, (char*)it->value);

	printf("my current girlfriend is: %s\n", (char*)hashmap_get(&h, "gf"));
	getchar(); return 0;
}
