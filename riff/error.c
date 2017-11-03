#include "error.h"

static char glob_error[ERROR_MAX_LEN];
static log_logger_t* glob_logger = NULL;
static unsigned glob_defprio = LOG_PRIO_ERROR;

void error_logger(log_logger_t* logger) {
	glob_logger = logger;
}

log_logger_t* error_logger_get(void) {
	return glob_logger;
};

void error_set_prio(unsigned log_prio) {
	glob_defprio = log_prio;
}

void error_set(const char* fmt, ...) {
	va_list li;
	va_start(li, fmt);

	vsnprintf(&glob_error[0], ERROR_MAX_LEN, fmt, li);
	glob_error[ERROR_MAX_LEN - 1] = 0;

	if (glob_logger)
		log_log(glob_logger, glob_defprio, "%s", glob_error);

	va_end(li);
}

void error_code(size_t code) {

	static const char* code_lut[] = {
#define XX(a, b) b,
	__ERROR_XMENUM
#undef XX
	};

	if (code < ERROR_OTHER)
		error_set(code_lut[code]);
	else
		error_set(code_lut[ERROR_OTHER]);
}

const char* error_get(void) {
	if (glob_error[0] == 0)
		return NULL;

	return &glob_error[0];
}

const char* error_gets(void) {
	const char* e = error_get();

	if (e)
		return e;
	else
		return "no error";
}

void error_clear(void) {
	glob_error[0] = 0;
}
