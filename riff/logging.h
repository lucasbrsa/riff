// @ make log priorities zero performance impact

#ifndef _LOGGING_H
#define _LOGGING_H

#include <stdio.h>

typedef unsigned char log_slt_key_t; // sequential log target
typedef unsigned char log_fm_flag_t; // flag modifier
typedef unsigned int log_cat_t; // category
typedef unsigned int log_prio_t; // priority

// format callback
typedef errno_t(*log_fc_f)(const char* i, char* o, size_t io_len, log_prio_t p, int ln, char* file, char* func);

// all possible sequential log targets
typedef enum {
	LOG_SLT_0 = 0,
	LOG_SLT_1,
	LOG_SLT_2,
	LOG_SLT_3,
	LOG_SLT_4,
	LOG_SLT_5,
	LOG_SLT_6,
	LOG_SLT_7,
	LOG_SLT_COUNT
} log_slts_t;

// internal state machine
typedef struct {
	FILE* files[LOG_SLT_COUNT];
	log_slt_key_t flag;
	log_fc_f callback;
} log_state_t;

// error codes
enum {
	LOG_ENONE = 0,
	LOG_EGENERIC = 1,
	LOG_EFORMAT,
	LOG_ENULL_FP,
	LOG_EFULL_TARGETS,
	LOG_EBAD_OPEN,
	LOG_EBAD_CLOSE
};

/* log raw data to active slt's */
void log_logr(const char* mbuf);

/* log data to active slt's, using the macro 'log' is recommended */
errno_t log_log(log_prio_t p, int ln, char* file, char* func, const char* buf);

/* log printf formatted data to active slt's, using the macro 'logf' is recommended */
errno_t log_logf(log_prio_t p, int ln, char* file, char* func, const char* fmt, ...);


/* setup the log state machine, opening 'fp' at LOG_SLT_0 */
void log_init(FILE* fp);

/* setup the log state machine, opening the path at LOG_SLT_0 */
errno_t log_initp(const char* path);

/* free the state machine and close all active files */
errno_t log_quit(void);


/* get the current fc */
log_fc_f log_fc_get(void);

/* set a new fc */
void log_fc_set(log_fc_f fc);

/* revert to the default fc */
void log_fc_reset(void);

/* the default fc */
errno_t log_fc_default(const char* i, char* o, size_t io_len, log_prio_t p, int ln, char* file, char* func);


/* store 'fp' the next available slt, returned in 'out' */
errno_t log_slt_openf(FILE* fp, log_slt_key_t* out);

/* store fp at the slt 'key', closing the lt stored at that key */
errno_t log_slt_openf_at(FILE* fp, log_slt_key_t key);

/* open 'path' the next available slt, returned in 'out' */
errno_t log_slt_open(const char* path, log_slt_key_t* out);

/* open 'path' at the slt 'key', closing the lt stored at that key */
errno_t log_slt_open_at(const char* path, log_slt_key_t key);

/* close the slt stored at 'key' */
errno_t log_slt_close_at(log_slt_key_t key);

/* close all active slt's */
errno_t log_slt_close_all(void);

/* get the FILE stored the key */
FILE* log_slt_get(log_slt_key_t key);

/* return the 'kb'th bit of the fm */
_Bool log_fm_get_bit(log_slt_key_t kb);

/* get the current fm */
log_fm_flag_t log_fm_get(void);

/* set the current fm */
void log_fm_set(log_fm_flag_t flag);

/* disable a certain slt key in the fm */
void log_fm_disable_at(log_slt_key_t key);

/* enable a certain slt key in the fm */
void log_fm_enable_at(log_slt_key_t key);

/* disable all slt's */
void log_fm_disable_all(void);

/* enable all slt's */
void log_fm_enable_all(void);


#ifndef LOG_WMODE
#	define LOG_WMODE "w"
#endif
#ifndef LOG_BUFF_SIZE
#	define LOG_BUFF_SIZE 256
#endif
#ifndef _MSC_VER
#	define __FUNCTION__ "<anonymous>"
#endif
#ifndef _WIN32
#	define LOG_DIRCHR '/'
#else
#	define LOG_DIRCHR '\\'
#endif
#define LOG_FILENAME (strrchr(__FILE__, LOG_DIRCHR) ? strrchr(__FILE__, LOG_DIRCHR) + 1 : __FILE__)


/* log printf formatted data to active slt's */
#define logf(fmt, ...) log_logf(0, __LINE__, LOG_FILENAME, __FUNCTION__, fmt, __VA_ARGS__)

/* log data to active slt's */
#define log(txt) log_log(0, __LINE__, LOG_FILENAME, __FUNCTION__, txt)

/* log raw data to active slt's */
#define logr(txt) log_logr(txt);

#endif // _LOGGING_H