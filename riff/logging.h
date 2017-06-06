// @ make log priorities zero performance impact
// @ allow for custom user setting rather than macros

#ifndef _LOGGING_H
#define _LOGGING_H

#include <string.h>
#include <stdio.h>

typedef unsigned char log_slt_key_t; // sequential log target
typedef unsigned char log_fm_flag_t; // flag modifier
typedef unsigned int log_cat_t; // category
typedef unsigned int log_prio_t; // priority

// format callback
typedef void(*log_fc_f)(const char* i, char* o, size_t io_len, log_cat_t c, log_prio_t p, int ln, char* file, char* func);

// all possible sequential log targets
// cannot have more than 8
typedef enum {
	LOG_SLT0 = 0,
	LOG_SLT1,
	LOG_SLT2,
	LOG_SLT3,
	LOG_SLT4,
	LOG_SLT5,
	LOG_SLT6,
	LOG_SLT7,
	LOG_SLT_COUNT
} log_slts_t;

// prios
typedef enum {
	/* NULL */
	LOG_PRIO_TRACE = 1,
	LOG_PRIO_DEBUG,
	LOG_PRIO_INFO,
	LOG_PRIO_WARN,
	LOG_PRIO_ERROR,
	LOG_PRIO_FATAL,
	LOG_PRIO_COUNT
} log_prio_t;

// cats
typedef enum {
	LOG_CAT_APPLICATION = 0,
	LOG_CAT_ASSERTION,
	LOG_CAT_TEST,

	LOG_CAT_ERROR,
	LOG_CAT_SYS,
	LOG_CAT_RENDER,
	
	LOG_CAT_RESERVED0,
	LOG_CAT_RESERVED1,
	LOG_CAT_RESERVED2,
	LOG_CAT_RESERVED3,
	LOG_CAT_RESERVED4,
	LOG_CAT_RESERVED5,
	LOG_CAT_RESERVED6,
	LOG_CAT_RESERVED7,

	LOG_CAT_CUSTOM0, /* add more custom cats here.. */
	LOG_CAT_COUNT
} log_cat_t;

// error codes
enum {
	LOG_ENONE = 0,
	LOG_EGENERIC,
	LOG_EFORMAT,
	LOG_ENULL_FP,
	LOG_EFULL_TARGETS,
	LOG_EBAD_OPEN,
	LOG_EBAD_CLOSE,
	LOG_EPASSED_EXEC
};

// singly linked list of caps, category associated priorities
struct log_cap_s {
	log_cat_t cat;
	log_prio_t prio;
	struct log_cap_s* next;
}; typedef struct log_cap_s log_cap_t;

// internal state machine
typedef struct {
	FILE* files[LOG_SLT_COUNT];
	log_slt_key_t flag;
	log_fc_f callback;
	log_cap_t* tail;
	log_prio_t capp, cassert, ctest;
} log_state_t;


/* log printf style formatted data to active slts, using the wrapper functions is recommended */
void log_message(log_cat_t c, log_prio_t p, int ln, char* file, char* func, const char* fmt, ...);

/* free the state machine, closing all active files */
void log_quit(void);

/* free and re-init the state machine, closing all active files */
void log_reset(void);


/* get the current fc */
log_fc_f log_fc_get(void);

/* set a new fc */
void log_fc_set(log_fc_f fc);

/* revert to the default fc */
void log_fc_reset(void);

/* the default fc */
void log_fc_default(const char* i, char* o, size_t io_len, log_cat_t c, log_prio_t p, int ln, char* file, char* func);


/* set all caps to use 'np' prio */
void log_cap_set_all(log_prio_t np);

/* set the selected 'cat' to use the prio 'prio', if none exists create one. */
void log_cap_set(log_prio_t prio, log_cat_t cat);

/* get the prio for that cat */
log_prio_t log_cap_get(log_cat_t cat);

/* reset all caps and use their defaults */
void log_cap_free(void);


/* store 'fp' the next available slt, returned in 'out' */
errno_t log_slt_open(FILE* fp, log_slt_key_t* out);

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


#endif // _LOGGING_H