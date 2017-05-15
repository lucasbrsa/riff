#include "logging.h"
#include "macro.h"

main() 
{
	log_init("../resources/riff.log");
	log("hello world!");
	log_quit();
	log_init("../resources/riff.log");
	log_init("../resources/liff.log");
	log("hello world!");
	log_quit();

	int u1, u2, u3, d;
	log_init("../resources/riff.log");
	log_open_target("../resources/liff.log", LOG_TARGET_1);
	log_open_target("../resources/miff.log", LOG_TARGET_2);
	log_open_next_target("../resources/siff.log", &u1);

	log("I am writing to 4 files simultaneously");
	
	log_open_next_target("../resources/ciff.log", &u2);
	log_open_next_target("../resources/eiff.log", &u3);
	log_open_target("../resources/jiff.log", LOG_TARGET_6);
	log_open_target("../resources/eiff.log", LOG_TARGET_7);
	if (log_open_next_target("../resources/yiff.log", &d))
		log("could not open file: ../resources/yiff.log ran out of targets");

	log("I am writing to 8 files simultaneously");
	log("seems to be working ^.^");

	log_reopen_target("../resources/piff.log", LOG_TARGET_6);

	log("jiff.log has now been redirected to piff.log");

	log_open_next_target("../resources/ziff.log", NULL); /* this should fail as all targets are being used */
	log_quit();

	return getchar();
}