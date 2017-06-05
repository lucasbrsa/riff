#include "logging.h"

#include <time.h>
#include <stdarg.h>
#include <malloc.h>
#include <string.h>
#include <assert.h>

/* internal state machine, stack allocated for now */
static log_state_t smachine = { 0 };

/* functions used internally that are otherwise useless */
_Bool   __log_slt_is_valid(log_slt_key_t key);
_Bool   __log_slt_is_std(log_slt_key_t key);
void	__log_fm_flip_bit(log_slt_key_t target);
void	__log_fm_zero_bit(log_slt_key_t target);
void	__log_fm_one_bit(log_slt_key_t target);


/* core */

void log_logr(char* mbuf) {
	log_slt_key_t it = 0, flag = smachine.flag;

	while (flag) {
		if ((flag & 1) && smachine.files[it])
			fputs(mbuf, smachine.files[it]);

		flag >>= 1;
		it++;
	}

	return LOG_ENONE;
}

errno_t log_log(log_prio_t p, int ln, char* file, char* func, const char* buf) {
	char post_format[LOG_BUFF_SIZE];
	if ((*smachine.callback)(buf, post_format, LOG_BUFF_SIZE, p, ln, file, func) != LOG_ENONE)
		return LOG_EFORMAT;
	
	log_logr(post_format);

	return LOG_ENONE;
}

errno_t log_logf(log_prio_t p, int ln, char* file, char* func, const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);

	char pre_format[LOG_BUFF_SIZE], post_format[LOG_BUFF_SIZE];
	vsnprintf(post_format, LOG_BUFF_SIZE, fmt, args);
	errno_t e = log_log(p, ln, file, func, post_format);
	va_end(args);
	return e;
}

void log_init(FILE* fp) {

	// would prevent memory leak in the case of double log_init
	// disabled for performance
	// log_quit();

	memset(&smachine, 0, sizeof(log_state_t));

	smachine.flag = 0xFF;
	smachine.callback = &log_fc_default;
	smachine.files[LOG_SLT_0] = fp;
}

errno_t log_initp(const char* path) { 
	FILE* fp = (path) ? fopen(path, LOG_WMODE) : NULL;
	if (!fp) return LOG_ENULL_FP;
	
	log_init(fp);
	return LOG_ENONE;
}

errno_t log_quit(void) {
	smachine.flag = 0x0;
	smachine.callback = NULL;
	return log_slt_close_all();
}


/* formatting callback */

log_fc_f log_fc_get(void) { return smachine.callback; }

void log_fc_set(log_fc_f fc) { smachine.callback = fc; }

void log_fc_reset(void) { log_fc_set(log_fc_default); }

errno_t log_fc_default(const char* i, char* o, size_t io_len, log_prio_t p, int ln, char* file, char* func) {
	//assert(p < LOG_LEVEL_COUNT && p >= 0);
	
	char str_time[9];
	char str_date[11];
	time_t t = time(NULL);

	strftime(str_time, sizeof(str_time), "%H:%M:%S", localtime(&t));
	strftime(str_date, sizeof(str_date), "%d-%m-%Y", localtime(&t));

	return (sprintf_s(o, io_len, "%s %s | %-5s | %s %s:%d | %s\n",
		str_date, str_time,
		"<DEBUG>", /* temp */
		file, func, ln, i) == -1) ? LOG_EFORMAT : LOG_ENONE;
}


/* sequential log targets */

errno_t log_slt_openf(FILE* fp, log_slt_key_t* out) {
	log_slt_key_t it;
	for (it = LOG_SLT_0; it < LOG_SLT_COUNT; it++) {
		if (!smachine.files[it]) {
			if (out != NULL)
				*out = it;

			smachine.files[it] = fp;
			return LOG_ENONE;
		}
	}

	return LOG_EFULL_TARGETS;
}

errno_t log_slt_open(const char* path, log_slt_key_t* out) {
	FILE* fp;
	if (fp = fopen(path, LOG_WMODE))
		return log_slt_openf(fp, out);
	return LOG_EBAD_OPEN;
}

errno_t log_slt_openf_at(FILE* fp, log_slt_key_t key) {
	assert(key < LOG_SLT_COUNT);

	if (smachine.files[key]) {
		if (!__log_slt_is_std(key))
			if (fclose(smachine.files[key]))
				return LOG_EBAD_CLOSE;

		smachine.files[key] = NULL;
	}

	smachine.files[key] = fp;
	return LOG_ENONE;
}

errno_t log_slt_open_at(const char* path, log_slt_key_t key) {
	FILE* fp = fopen(path, LOG_WMODE);
	if (!fp)
		return LOG_EBAD_OPEN;

	return log_slt_openf_at(fp, key);
}

errno_t log_slt_close_at(log_slt_key_t key) {
	assert(key < LOG_SLT_COUNT);

	if (!__log_slt_is_valid(key))
		if (fclose(smachine.files[key]))
			return LOG_EBAD_CLOSE;

	smachine.files[key] = NULL;
	return LOG_ENONE;
}

errno_t log_slt_close_all(void) {
	int err = 0;
	log_slt_key_t it;

	for (it = LOG_SLT_0; it < LOG_SLT_COUNT; it++) {
		if (__log_slt_is_valid(it))
			err |= fclose(smachine.files[it]);

		smachine.files[it] = NULL;
	}

	return (!err)?LOG_ENONE:LOG_EBAD_CLOSE;
}


/* functions used internally that are otherwise useless */

_Bool __log_slt_is_std(log_slt_key_t key) {
	return (
		smachine.files[key] == stdout ||
		smachine.files[key] == stderr ||
		smachine.files[key] == stdin);
}

_Bool __log_slt_is_valid(log_slt_key_t key) {
	return (smachine.files[key] && !__log_slt_is_std(key));
}

FILE* log_slt_get(log_slt_key_t key) { return smachine.files[key]; }


/* log flag */

_Bool log_fm_get_bit(log_slt_key_t kb) { return ((1 << kb) & smachine.flag) > 0; }

log_fm_flag_t log_fm_get(void) { return smachine.flag; }

void log_fm_set(log_fm_flag_t flag) { smachine.flag = flag; }

void log_fm_disable_at(log_slt_key_t key_target) { __log_fm_zero_bit(key_target); }

void log_fm_enable_at(log_slt_key_t key_target) { __log_fm_one_bit(key_target); }

void log_fm_disable_all(void) { smachine.flag = 0x00; }

void log_fm_enable_all(void) { smachine.flag = 0xFF; }


/* modify bits of the flag */

void __log_fm_flip_bit(log_slt_key_t target) { smachine.flag ^= 1 << target; }

void __log_fm_zero_bit(log_slt_key_t target) { smachine.flag &= ~(1 << target); }

void __log_fm_one_bit(log_slt_key_t target) { smachine.flag |= 1 << target; }