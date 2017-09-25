#include <stdio.h>

#include "log.h"

int main(int argc, char ** argv) {
	log_msg_t m;
	m.in = "example";

	log_fmt_t* f = log_compile_pattern("%m here is a very simple %m pattern...%m");
	log_test(f, &m);
	puts(m.out);
	log_free_pattern(f);
}
