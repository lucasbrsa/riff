#include "logging.h"

#include <stdio.h>
#include <time.h>

main() 
{
	FILE* fp = fopen("../resources/_proc.log", "w");
	if (!fp) 
	{
		puts("failed to open file");
		return getchar();
	}

	add_log_target_file(LOG_FILE_TARGET_0, fp);
	add_log_target_file(LOG_FILE_TARGET_7, stdout);
	set_log_target_flag( (1 << LOG_FILE_TARGET_0) | (1 << LOG_FILE_TARGET_7) );

	time_t t;
	struct tm* t_info;

	time(&t);
	t_info = localtime(&t);
	
	logf("the time is: %s and it's a beautiful day!", asctime(t_info));
	log("my name is riff - i am a world changing 3d renderer that supports:\n");
	log("  - logging");
	log("  - os checking");
	log("  - \\0");

	clear_log_target_files();
	set_log_target_flag(0x00);

	fclose(fp);

	return getchar();
}