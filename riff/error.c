#include "error.h"
#include "str.h"
#include "vector.h"

static vector_t* glob_stack = NULL;
static bool glob_silent = false;

static const char* glob_code_lut[] = {
#define XX(a, b) b,
	ERROR_CODES
#undef XX
};

void error_set_silent(bool s) {
	glob_silent = s;
}

error_t* error_form_estruct(
		size_t code,
		size_t line,
		const char* file,
		const char* func,
		const char* fmt, ...) {

	va_list li;
	va_start(li, fmt);

	error_t* e = malloc(sizeof(error_t));

	e->code = (code <= ERROR_OTHER)? code : ERROR_OTHER;
	e->line = line;
	e->file = file;
	e->func = func;

	if (fmt)
		vsnprintf(&e->msg[0], sizeof(e->msg), fmt, li);
	else
		str_cpy(&e->msg[0], glob_code_lut[e->code]);

	e->msg[sizeof(e->msg) - 1] = 0;

	va_end(li);

	return e;
}

void error_set_newerror(error_t* error) {
	if (glob_stack == NULL)
		glob_stack = vector_init(2, sizeof(error_t*), free);

	if (!glob_silent) {
		log_wrapper(
				error->func, error->file, error->line,
				ERROR_PRIO, ERROR_FMT, error->msg);
	}

	vector_push_back(glob_stack, error);
}

size_t error_get_depth(void) {
	return (glob_stack == NULL)? 0 : vector_size(glob_stack);
}

error_t* error_get_struct(size_t index) {
	if (glob_stack == NULL)
		return NULL;

	return vector_att(glob_stack, index, error_t*);
}

void error_log(void) {
	if (glob_stack == NULL)
		return;

	for (int i = 0; i < vector_size(glob_stack); i++) {
		error_t* e = vector_att(glob_stack, i, error_t*);
		log_wrapper(e->func, e->file, e->line, ERROR_PRIO, ERROR_FMT, e->msg);
	}
}

void error_clear(void) {
	if (glob_stack)
		vector_free(glob_stack);
}

const char* error_get(void) {
	if (glob_stack == NULL || vector_size(glob_stack) == 0)
		return "no error";

	return error_get_struct(vector_size(glob_stack) - 1)->msg;
}
