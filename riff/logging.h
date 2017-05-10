#ifndef _LOGGING_H
#define _LOGGING_H

#include "macro.h"

#include <stdio.h> /* for FILE* */

#define MAX_LOGF_BUFFER_LENGTH 128

/* assigns a file ptr to a LOG_FILE_TARGET */
void add_log_target_file(int log_file_target, FILE* file_ptr);

/* sets a LOG_FILE_TARGET to null */
/* warning: this does not close the file pointers for you! */
void del_log_target_file(int log_file_target);

/* sets all LOG_FILE_TARGETs to null */
/* warning: this does not close the file pointers for you! */
void clear_log_target_files(void);

/* each bit of the flag states weather a specific log target should be written to (byte order, right to left) */
/* example: 0000 0101 */
/* would log to: LOG_FILE_TARGET_0 && LOG_FILE_TARGET_2 */
/* example 2: if one wanted to use LOG_FILE_TARGET_2 and LOG_FILE_TARGET_6: */
/* set_log_target_flag((1 << LOG_FILE_TARGET_2) | ((1 << LOG_FILE_TARGET_6)); */
void set_log_target_flag(unsigned char flag);

/* sets a certain bit (target) of the log target flag to zero */
void zero_log_target_flag_file(unsigned char target);

/* sets a certain bit (target) of the log target flag to one */
void one_log_target_flag_file(unsigned char target);

/* flips a certain bit (target) of the log target flag */
void flip_log_target_flag_file(unsigned char target);

/* disables all file logging by setting log target to 0000 0000 */
void zero_log_target_flag(void);

/* enables logging to all targets by setting log target to 1111 1111 */
void one_log_target_flag(void);

/* adds file to next available file target (if possible) and ones that flag bit */
/* returns the LOG_FILE_TARGET or -1 on failure */
int add_log_file(FILE* file_ptr);

/* formatted logging to log files selected by the log_target_flag */
int logf(const char* fmt, ...);

/* logging to log files selected by the log_target_flag */
int log(const char* cchar);

/* returns the current log target flag */
unsigned char get_log_target_flag(void);

/* returns the FILE* or NULL stored at a target */
FILE* get_log_target(unsigned char target);

/* allows for a single log to but directed to multiple FILEs */
/* to put into form of log target flag: (1 << LOG_FILE_TARGET) */
enum
{
	LOG_FILE_TARGET_0 = 0,
	LOG_FILE_TARGET_1,
	LOG_FILE_TARGET_2,
	LOG_FILE_TARGET_3,
	LOG_FILE_TARGET_4,
	LOG_FILE_TARGET_5,
	LOG_FILE_TARGET_6,
	LOG_FILE_TARGET_7, /* generally used for stdout */
	LOG_FILE_TARGET_COUNT
};

#endif