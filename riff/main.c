#include "logging.h"
#include "macro.h"

main() 
{
	log_init(NULL);
	
	log_open_next_target("../resources/1.log", NULL);
	log_open_next_target("../resources/2.log", NULL);
	log_open_next_target("../resources/3.log", NULL);
	log_open_next_target("../resources/4.log", NULL);
	log_open_next_target("../resources/5.log", NULL);
	
	LOG("hello");

	log_quit();

	return getchar();
}