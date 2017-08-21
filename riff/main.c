#include <malloc.h>
#include <stdio.h>
#include "log.h"

int main(int argc, char** argv) {
	log_fmt_t p = log_compile_pattern("[%c:%t] %% %m");
	log_msg_t* m = malloc(sizeof(log_msg_t));
	
	m->cat = 0;
	m->message = "smd";
	
	log_print_fmt(m, p);
	log_free_fmt(&p);
	free(m);

	return getchar();
}