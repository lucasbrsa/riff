#ifndef _LOG_H
#define _LOG_H

#include <stdio.h>

/* category - lookups that identify the logs place in the program */
/* format - the output format string */
/* rules - the map of categories to their targets and how it should look */
/* configuration is split into two sections: global and per logger */

/* global configuration */

typedef char* (*log_fmtcback)(void);

typedef union {
	long long i;
	char* c;
} log_typepair;

typedef struct {
	union {
		log_typepair* p;
		log_fmtcback c;
	}*lut;
} log_cfmt_t;

/*
* %c catagory
* %d date
* %e custom environment variables
* %f file
* %i the process id
* %l line
* %m function
* %n newline character
* %o output filename
* %p level
* %s the log message
* %t time
* %% percent sign
*/
log_cfmt_t log_compile_pattern(const char* fmt);

void log_free_pattern(log_cfmt_t* const patt);

errno_t log_set_pattern(const log_cfmt_t* patt);

void log_reset_pattern(void);

void log_set_time_format(const char* fmt);

void log_set_date_format(const char* fmt);

/* temporary, for demonstrating the format meme */
typedef struct {
	const char* fmt;
} logger_t;

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