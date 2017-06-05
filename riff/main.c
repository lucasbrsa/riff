#include "logging.h"

main() {
	log_initp("../resources/proc.txt");
	log_slt_openf(stderr, NULL);
	
	log("hello world!");
	logf("compiled at: %s", __TIME__);

	getchar();
	return log_quit();
}