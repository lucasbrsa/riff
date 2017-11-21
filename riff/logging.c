#include "logging.h"
#include "generic.h"
#include "str.h"

#include <string.h>
#include <stdarg.h>

#if LOGGING_USE_SYSLOG == 1
#include <syslog.h>
#endif

/* the logger state machine properties */
static logging_writer_f global_writer = LOGGING_DEFAULT_WRITER;
static unsigned global_level = LOGGING_DEBUG;
static unsigned global_msgcount = 0;
static FILE* global_filetarget = NULL;

#ifdef LOGGING_SILENT
static bool global_silent = true;
#else
static bool global_silent = false;
#endif

static const char* levels[] = {
	"DEBUG", "INFO", "NOTICE", "WARN", "ERROR", "PANIC"
};

void logging_wrapper(
		const char* function,
		const char* file,
		unsigned line,
		unsigned level,
		const char* fmt, ...) {

	if (level < global_level || fmt == NULL) {
		return;
	}

	va_list args;
	va_start(args, fmt);

	time_t t; time(&t);
	struct tm* lt = localtime(&t);
	static char timebuf[LOGGING_TIME_FMTL+1];
	strftime(&timebuf[0], sizeof(timebuf), LOGGING_TIME_FMT, lt);

	level = (level > LOGGING_PANIC)? LOGGING_PANIC : level;

	char* msg = str_vsprintf(fmt, args);

	/* char* cleanpath = malloc(strlen(file)); */
	/* str_clean_path(cleanpath, file); */

	const char* cleanpath = file;

	static logging_msg_t userdata;
	userdata.file = cleanpath;
	userdata.ftime = &timebuf[0];
	userdata.func = function;
	userdata.id = global_msgcount++;
	userdata.level = level;
	userdata.line = line;
	userdata.message = str_strip(msg);
	userdata.tinfo = lt;

	if (global_writer)
		global_writer(&userdata);

	free(msg);

	/* free(cleanpath); */

	va_end(args);
}

void logging_writer_coloured(logging_msg_t* msg) {
	static const char* level_colours[] = {
		OS_COLOUR_BRIGHT4, OS_COLOUR_CYAN, OS_COLOUR_BLUE,
		OS_COLOUR_YELLOW, OS_COLOUR_RED, OS_COLOUR_MAGENTA
	};

	if (global_silent)
		return;

	fprintf(stderr, "%s %s%-6s " OS_COLOUR_BRIGHT0 "%s:%02d " OS_COLOUR_WHITE "%s\n",
		msg->ftime, level_colours[msg->level], levels[msg->level],
		msg->file, msg->line, msg->message);

	fprintf(stderr, OS_COLOUR_RESET);
}

void logging_writer_stderr(logging_msg_t* msg) {
	if (global_silent)
		return;

	fprintf(stderr, "%s %-6s %s:%02d %s\n",
		msg->ftime, levels[msg->level], msg->file, msg->line, msg->message);
}

void logging_writer_stdout(logging_msg_t* msg) {
	if (global_silent)
		return;

	printf("%s %-6s %s:%02d %s\n",
		msg->ftime, levels[msg->level], msg->file, msg->line, msg->message);
}

void logging_writer_file(logging_msg_t* msg) {
	if (global_filetarget) {
		fprintf(global_filetarget, "%s %-6s %s:%02d %s\n",
			msg->ftime, levels[msg->level], msg->file, msg->line, msg->message);
		fflush(global_filetarget);
	}
}

void logging_writer_syslog(logging_msg_t* msg) {
#if LOGGING_USE_SYSLOG == 1
	syslog(LOG_DEBUG - msg->level, "%s", msg->message);
#endif
}

void logging_writer_colourful_and_file(logging_msg_t* msg) {
	logging_writer_coloured(msg);
	logging_writer_file(msg);
}

void logging_set_level(unsigned l) {
	global_level = l;
}

void logging_set_file(FILE* fp) {
	global_filetarget = fp;
}

void logging_set_writer(logging_writer_f writer) {
	global_writer = writer;
}

void logging_get_writer(logging_writer_f* writer) {
	if (writer) {
		*writer = global_writer;
	}
}

FILE* logging_get_file(void) {
	return global_filetarget;
}

unsigned logging_get_level(void) {
	return global_level;
}

void logging_set_silent(void) {
	global_silent = true;
}

void logging_set_noisy(void) {
	global_silent = false;
}

unsigned logging_get_count(void) {
	return global_msgcount;
}
