#ifndef _LOG_H
#define _LOG_H

#include <stdio.h>

/* global configuration */
/* category - lookups that identify the logs place in the program */
/* format - the output format string */
/* rules - the map of categories to their targets and how it should look */
/* configuration is split into two sections: global and per logger */

/* temporary, for demonstrating the format meme */
typedef struct {
	char *in, *out;
} log_msg_t;

typedef void (*log_fmt_cback)(log_msg_t* msg);

/* would much rather use anonymous function */
typedef struct {
	char* leftof;
	log_fmt_cback c;
} log_fmt_stpair_t;

typedef struct {
	log_fmt_stpair_t* stack;
	char* left_pool;
	unsigned short len;
} log_fmt_t;

/*
* %c catagory
* %e custom environment variables
* %f file
* %i the process id
* %l line
* %m function
* %n newline character
* %o output filename
* %p level
* %s the log message
* %t date time
* %% percent sign
*/
log_fmt_t log_compile_pattern(const char* fmt);

log_fmt_cback log_interpret_fmt_flag(char flg);

void log_free_fmt(log_fmt_t* const patt);

/* tmp demonstration kode with karlie */
void log_print_fmt(log_msg_t* msg, log_fmt_t fmt);

/* ehh, just shit that modifies the state machine... not really important */
void log_set_fmt(const log_fmt_t* patt);

void log_set_pattern(const char* fmt);

void log_set_timepattern(const char* patt);

void log_reset_pattern(void);

void log_reset_timepattern(void);

//void log_set_level(void);

//void log_gget(const char* name); /* return the logger attached to the name */

/* individual loggers */

//void log_create(const char* name); /* create a new logger */


/* wrapper functions */

//void log_create_basic(const char* name, FILE* file, int truncation);

//void log_create_stdout(const char* name);

//void log_create_stderr(const char* name);

//void log_create_stdout_col(const char* name);

//void log_create_stderr_col(const char* name);

/* writers are actually responsible for writing the log text */

#endif