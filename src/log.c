#include "log.h"
#include "generic.h"
#include "str.h"

#include <string.h>
#include <stdarg.h>

#if LOG_USE_SYSLOG == 1
# 	include <syslog.h>
#endif

#if 1 /* HAS_UNISTD */
# 	include <unistd.h>
#endif

/* the logger state machine properties */
static log_writer_f glob_writer = LOG_DEFAULT_WRITER;
static unsigned glob_level = LOG_LEVEL_DEBUG;
static unsigned glob_msgcount = 0;
static FILE* glob_filetarget = NULL;

#ifdef LOG_SILENT
static bool glob_silent = true;
#else
static bool glob_silent = false;
#endif

static const char* levels[] = {
	"DEBUG", "INFO", "NOTICE", "WARN", "ERROR", "PANIC"
};

void log_wrapper(
		const char* function,
		const char* file,
		unsigned line,
		unsigned level,
		const char* fmt, ...) {

	if (level < glob_level || fmt == NULL) {
		return;
	}

	va_list args;
	va_start(args, fmt);

	time_t t; time(&t);
	struct tm* lt = localtime(&t);
	static char timebuf[LOG_TIME_FMTL+1];
	strftime(&timebuf[0], sizeof(timebuf), LOG_TIME_FMT, lt);

	level = (level > LOG_LEVEL_PANIC)? LOG_LEVEL_PANIC : level;

	char* msg = str_vsprintf(fmt, args);

	/* char* cleanpath = malloc(strlen(file)); */
	/* str_clean_path(cleanpath, file); */

	const char* cleanpath = file;

	static log_msg_t userdata;
	userdata.file = cleanpath;
	userdata.ftime = &timebuf[0];
	userdata.func = function;
	userdata.id = glob_msgcount++;
	userdata.level = level;
	userdata.line = line;
	userdata.message = str_strip(msg);
	userdata.tinfo = lt;

	if (glob_writer)
		glob_writer(&userdata);

	free(msg);

	/* free(cleanpath); */

	va_end(args);
}

void log_writer_coloured(log_msg_t* msg) {
	static const char* level_colours[] = {
		OS_COLOUR_BRIGHT4, OS_COLOUR_CYAN, OS_COLOUR_BLUE,
		OS_COLOUR_YELLOW, OS_COLOUR_RED, OS_COLOUR_MAGENTA
	};

	if (glob_silent)
		return;

	fprintf(stderr, "%s %s%-6s " OS_COLOUR_BRIGHT0 "%s:%02d " OS_COLOUR_WHITE "%s\n",
		msg->ftime, level_colours[msg->level], levels[msg->level],
		msg->file, msg->line, msg->message);

	fprintf(stderr, OS_COLOUR_RESET);
}

void log_writer_stderr(log_msg_t* msg) {
	if (glob_silent)
		return;

	fprintf(stderr, "%s %-6s %s:%02d %s\n",
		msg->ftime, levels[msg->level], msg->file, msg->line, msg->message);
}

void log_writer_stdout(log_msg_t* msg) {
	if (glob_silent)
		return;

	printf("%s %-6s %s:%02d %s\n",
		msg->ftime, levels[msg->level], msg->file, msg->line, msg->message);
}

void log_writer_file(log_msg_t* msg) {
	if (glob_filetarget) {
		fprintf(glob_filetarget, "%s %-6s %s:%02d %s\n",
			msg->ftime, levels[msg->level], msg->file, msg->line, msg->message);
		fflush(glob_filetarget);
	}
}

void log_writer_syslog(log_msg_t* msg) {
#if LOG_USE_SYSLOG == 1
	syslog(LOG_DEBUG - msg->level, "%s", msg->message);
#endif
}

void log_writer_colourful_and_file(log_msg_t* msg) {
	log_writer_coloured(msg);
	log_writer_file(msg);
}

void log_set_level(unsigned l) {
	glob_level = l;
}

void log_set_file(FILE* fp) {
	glob_filetarget = fp;
}

void log_set_writer(log_writer_f writer) {
	glob_writer = writer;
}

void log_get_writer(log_writer_f* writer) {
	if (writer) {
		*writer = glob_writer;
	}
}

FILE* log_get_file(void) {
	return glob_filetarget;
}

unsigned log_get_level(void) {
	return glob_level;
}

void log_set_silent(void) {
	glob_silent = true;
}

void log_set_noisy(void) {
	glob_silent = false;
}

unsigned log_get_count(void) {
	return glob_msgcount;
}
