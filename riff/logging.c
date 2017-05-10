#include "logging.h"

#include <malloc.h>
#include <stdarg.h>

/* underlying static file targets and file target flag */
FILE* base_log_targets[LOG_FILE_TARGET_COUNT];
unsigned char base_log_target_flag = 0x00;

/* assigns a file ptr to a LOG_FILE_TARGET */
void add_log_target_file(int log_file_target, FILE* file_ptr) { base_log_targets[log_file_target] = file_ptr; }

/* sets a LOG_FILE_TARGET to null */
void del_log_target_file(int log_file_target) { base_log_targets[log_file_target] = NULL; }

/* sets all LOG_FILE_TARGETs to null */
void clear_log_target_files(void)
{
	int i;
	for (i = 0; i < LOG_FILE_TARGET_COUNT; i++)
		base_log_targets[i] = NULL;
}

/* each bit of the flag states weather a specific log target should be written to (byte order, right to left) */
void set_log_target_flag(unsigned char flag) { base_log_target_flag = flag; }

/* sets a certain bit (target) of the log target flag */
void zero_log_target_flag_file	(unsigned char target) { base_log_target_flag &= ~(1 << target); }
void one_log_target_flag_file	(unsigned char target) { base_log_target_flag |= 1 << target; }
void flip_log_target_flag_file	(unsigned char target) { base_log_target_flag ^= 1 << target; }

/* formatted logging to log files selected by the log_target_flag */
int logf(const char* fmt, ...)
{
	char* buffer = malloc(MAX_LOGF_BUFFER_LENGTH);
	
	va_list args;
	va_start(args, fmt);
	vsprintf_s(buffer, MAX_LOGF_BUFFER_LENGTH, fmt, args);
	va_end(args);

	log(fmt);
	free(buffer);
}

/* logging to log files selected by the log_target_flag */
int log(const char* cchar)
{
	unsigned char it = 0, flag = base_log_target_flag;

	while (flag)
	{
		if (flag & 1) {
			fputs(cchar, base_log_targets[it]);
			fputc('\n', base_log_targets[it]);
		}

		flag >>= 1;
		it++;
	}
}