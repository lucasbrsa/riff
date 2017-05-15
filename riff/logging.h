// Important:
// @TODO check for stdout/err/in so these cannot be closed prematurely
// @TODO add intermediate log buffer : queue (array) of strings
// @TODO add categories so certain logging can be disabled
// @TODO allow for custom logger_state_t's to be used rather than static
//
// Minor:
// @TODO prevent potential memory leak from repeat calls of log_init
// @TODO add safety for flag modifier functions

/* The logging system uses an errno_t function and exception design principal.
 * This is because aspects of the exception system cannot work without the logging system
 * A chicken before the egg problem 
 */

#ifndef _LOGGING_H
#define _LOGGING_H

#include <stdio.h>

typedef unsigned int tkey_t; /* used to represent an index in the flag byte */
typedef unsigned char flag_t; /* used for flag byte */

/* each log target links a bit of the target_flag with the target_files array */
/* this allows for a single log to be directed to multiple FILEs */
/* to put into bit form (1 << LOG_TARGET_N) */
typedef enum {
	LOG_TARGET_0 = 0,
	LOG_TARGET_1,
	LOG_TARGET_2,
	LOG_TARGET_3,
	LOG_TARGET_4,
	LOG_TARGET_5,
	LOG_TARGET_6,
	LOG_TARGET_7, /* stdout */
	LOG_TARGET_COUNT
} log_target_t;

/* container struct for the logger state machine */
typedef struct {
	flag_t target_flag; /* number where each bit represents weather the associated TARGET should be logged to */
	FILE* target_files[LOG_TARGET_COUNT]; /* an array of FILEs that the logger will write to, NULLs are skipped */
} logger_state_t;

/* struct that contains any data relevant to the calling context of a log */
typedef struct {
	char *file, *func;
	int line;
} caller_info_t;

/* --- operating on the FILE targets --- */

/* initializes the built in log_state_t for logging */
/* returns !SUCCESS if the file could not be opened */
/* LOG_TARGET_0 stores fopen(path) */
errno_t log_init(const char* path);

/* close and NULL all currently open log targets while resetting the logger_state_t */
/* this also resets the log target flag to 0000 0000 */
/* returns !SUCCESS if one of the files could not be closed */
errno_t log_quit(void);

/* opens a file at the key */
/* returns FAILURE if the file could no be opened or there was already a file at that slot */
errno_t log_open_target(const char* path, tkey_t key);

/* opens a file in the next available LOG_TARGET which is placed in *out */
/* returns FAILURE if the file could not be opened or all TARGETS are full */
errno_t log_open_next_target(const char* path, tkey_t* out);

/* reopens a file based on the path in the key, if there is no file already present the open will persist */
/* returns !SUCCESS if the file could not be opened or the existing file could not be closed */
errno_t log_reopen_target(const char* path, tkey_t key);

/* links an existing FILE with a target key */
/* warning: will override any file at that key */
/* will return FAILURE if the file has been overridden */
errno_t log_add_target(tkey_t key_target, FILE* file_ptr);

/* will safely close the files stored at the key_target */
/* returns FAILURE if the file could not be closed. */
errno_t log_close_target(tkey_t key_target);

/* will safely close the all currently open files, somewhat similar to log_quit */
/* returns FAILURE if one of more of the files could not be closed. */
errno_t log_close_targets(void);

/* removes a link between a key and FILE target, this will not close the file use log_close_target for that */
void log_del_target(tkey_t key_target);

/* will remove all target files, but will not close then use log_close_targets for that */
void log_del_targets(void);


/* --- operating on the target flag --- */

/* disable all logging from a specific target without closing it */
void log_disable_target(tkey_t key_target);

/* enable all logging from a specific target without closing it */
void log_enable_target(tkey_t key_target);

/* enable logging from all file targets */
void log_disable(void);

/* disable logging from all file targets */
void log_enable(void);

/* each bit of the flag states weather a specific log target should be written to (byte order, right to left) */
/* for example 0000 0101 would only log to LOG_FILE_TARGET_0 and LOG_FILE_TARGET_2 */
/* thus use the formula (1 << LOG_FILE_TARGET_0) | (1 << LOG_FILE_TARGET_1) ... */
void log_set_tflag(flag_t flag);

/* get the state of a specific bit in the target flag */
_Bool log_get_tflag_bit(tkey_t target);

/* get the current target flag */
flag_t log_get_tflag(void);

/* for a specific key find the FILE currently accosted with that */
/* will return NULL when there is no association */
FILE* log_get_target(tkey_t key_target);

/* --- core functions --- */

//void logf(const char* fmt, ...);
void log(const char* data);


/* functions used internally that are otherwise useless */

errno_t __log_openq_target(const char* path, tkey_t key);
_Bool __log_is_valid_target(tkey_t key_target);
void __log_flip_target_flag_bit(flag_t target);
void __log_zero_target_flag_bit(flag_t target);
void __log_one_target_flag_bit(flag_t target);

#endif