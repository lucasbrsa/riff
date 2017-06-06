#include "logging.h"
#include "logdef.h"

main() {
	log_slt_open(fopen("../resources/proc.txt", "w"), NULL);
	log_slt_open(stdout, NULL);

	log_trace(LCAPP, "log");
	log_debug(LCAPP, "log");
	log_info(LCAPP, "log");
	log_warn(LCAPP, "log");
	log_error(LCAPP, "log");
	log_fatal(LCAPP, "log");

	log_cap_set(LPERR, LCREND);

	log_trace(LCREND, "log");
	log_debug(LCREND, "log");
	log_info(LCREND, "log");
	log_warn(LCREND, "log");
	log_error(LCREND, "log");
	log_fatal(LCREND, "log");

	log_quit();
	getchar();
}