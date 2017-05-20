#include "logging.h"
#include "macro.h"

#include <stdarg.h>
#include <malloc.h>
#include <string.h>

/* internal state machine - stack allocated for now */
static logger_state_t smachine = { 0 };

/* setup the smachine */
errno_t log_init(const char* path) {
	/* would prevent memory leak in the case of double log_init but may hinder performance: */
	log_quit();

	memset(&smachine, 0, sizeof(logger_state_t));

	smachine.target_flag = 0xFF;
	smachine.category_flag = 0xFF;
	smachine.callback = &__log_scallback;

	smachine.target_files[LOG_TARGET_7] = stdout;
	smachine.target_files[LOG_TARGET_0] = (path)?fopen(path, LOG_OPEN_MODE):NULL;

	return (errno_t)smachine.target_files[LOG_TARGET_0];
}

/* close all open files */
errno_t log_quit(void) {
	smachine.target_flag = 0x0;
	return log_close_targets();
}

/* opens a file */
errno_t log_open_target(const char* path, tkey_t key) {
	if (!smachine.target_files[key]) 
		return (__log_openq_target(path, key)) ? FAILURE : SUCCESS;

	return FAILURE;
}

/* open a new file in the next slot */
errno_t log_open_next_target(const char* path, tkey_t* out) {
	tkey_t it;
	for (it = LOG_TARGET_0; it < LOG_TARGET_COUNT; it++) {
		if (!smachine.target_files[it]) {
			if(out != NULL)	*out = it;
			return __log_openq_target(path, it);
		}
	}

	return FAILURE;
}

/* try to reopen the specified file */
errno_t log_reopen_target(const char* path, tkey_t key) {
	if (__log_is_valid_target(key)) {
		if (fclose(smachine.target_files[key])) return FAILURE;
		
		smachine.target_files[key] = NULL;
	}
	
	return __log_openq_target(path, key);
}

/* links FILE and a key
errno_t log_add_target(tkey_t key_target, FILE* file_ptr) {
	errno_t is = smachine.target_files[key_target];
	smachine.target_files[key_target] = file_ptr;
	return is;
} *

/* close a target file */
errno_t log_close_target(tkey_t key_target) { 
	if (fclose(smachine.target_files[key_target]))
		return FAILURE;

	smachine.target_files[key_target] = NULL;
	return SUCCESS;
}

/* close all open files */
errno_t log_close_targets(void) {
	errno_t r = SUCCESS;
	tkey_t it;
	for (it = LOG_TARGET_0; it < LOG_TARGET_COUNT; it++) {
		if (__log_is_valid_target(it))
			r |= fclose(smachine.target_files[it]);

		smachine.target_files[it] = NULL;
	}

	return r;
}

/* remove a key target pair */
void log_del_target(tkey_t key_target) {
	smachine.target_files[key_target] = NULL;
}

/* remove all key target pairs */
void log_del_targets(void) {
	tkey_t it;
	for (it = LOG_TARGET_0; it < LOG_TARGET_COUNT; it++)
		smachine.target_files[it] = NULL;
}

/* quicker file open */
errno_t __log_openq_target(const char* path, tkey_t key) {
	smachine.target_files[key] = fopen(path, LOG_OPEN_MODE);
	return !smachine.target_files[key];
}

/* is a file target applicable for closing */
_Bool __log_is_valid_target(tkey_t key_target) {
	return (smachine.target_files[key_target] &&
		/* there should be a better solution for this... */
		smachine.target_files[key_target] != stdout &&
		smachine.target_files[key_target] != stderr &&
		smachine.target_files[key_target] != stdin);
}

/* flag modifiers */

void log_disable_target(tkey_t key_target) { __log_zero_target_flag_bit(key_target); }

void log_enable_target(tkey_t key_target) { __log_one_target_flag_bit(key_target); }

void log_set_tflag(flag_t flag) { smachine.target_flag = flag; }

void log_disable(void) { smachine.target_flag = 0x00; }

void log_enable(void) { smachine.target_flag = 0xFF; }

_Bool log_get_tflag_bit(tkey_t target) { return ((1 << target) & smachine.target_flag) > 0; }

flag_t log_get_tflag(void) { return smachine.target_flag; }

FILE* log_get_target(tkey_t key_target) { return smachine.target_files[key_target]; }


/* core logging system */
void log_set_callback(log_callback_f lc) { smachine.callback = lc; }

void log_reset_callback(void) { smachine.callback = &__log_scallback; }

void __log_scallback(const char* in, caller_info_t caller_info, int out_len, char* out) {
	out = malloc(out_len);
	char* t_buffer = malloc(28);

	strftime(t_buffer, 28, "3", &caller_info.call_time);

	sprintf_s(out, out_len, "INFO[%04i] %s\t%s:%i\n",
		caller_info.call_num, in, caller_info.func, caller_info.line);

	return;
}

void log(char* file, int len_file, char* func, int len_func, int line, const char* data) {

	time_t timer;
	time(&timer);
	struct tm* t = localtime(&timer);

	char* output = malloc(LOG_BUFF_SIZE);
	smachine.callback(data, (caller_info_t) {
		len_file, len_func, line, smachine.log_number,
		file, func,
		t }, LOG_BUFF_SIZE, output);
	
	tkey_t it = 0, flag = smachine.target_flag;

	while (flag)
	{
		if ((flag & 1) && smachine.target_files[it]) {
			fputs(output, smachine.target_files[it]);
		}

		flag >>= 1;
		it++;
	}

	free(output);
	smachine.log_number++;
}

void logf(char* file, int len_file, char* func, int len_func, int line, const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);

	char* buffer = malloc(LOG_BUFF_SIZE);
	vsprintf(buffer, fmt, args);
	
	log(file, len_file, func, len_func, line, buffer);

	free(buffer);
	va_end(args);
}

/* modify bits of the flag */

void __log_flip_target_flag_bit(flag_t target) { smachine.target_flag ^= 1 << target; }

void __log_zero_target_flag_bit(flag_t target) { smachine.target_flag &= ~(1 << target); }

void __log_one_target_flag_bit(flag_t target) { smachine.target_flag |= 1 << target; }