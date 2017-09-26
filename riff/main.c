#include <stdio.h>
#include <time.h>

#include "log.h"

int main(int argc, char ** argv) {
	log_msg_t m;
	m.in = "example";

	time_t t;
	time(&t);
	m.tinfo = localtime(&t);

	log_fmt_t f = log_compile_pattern("%%%m%m%m%h%t%m%%%%%%%%m");
	log_test(f, &m);
	puts(m.out);
	log_free_pattern(f);
}
