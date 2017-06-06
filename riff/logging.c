#include "logging.h"

#include <time.h>
#include <stdarg.h>
#include <malloc.h>
#include <string.h>
#include <assert.h>

#define LOG_CAT_NAME_BUFF 12
#define LOG_SLT_BUFF_SIZE 256
#define LOG_SLT_OPEN_MODE "w"
#define LOG_PRIO_DEFAULT LOG_PRIO_INFO
#define LOG_PRIO_DEFAULT_TEST LOG_PRIO_DEBUG
#define LOG_PRIO_DEFAULT_ASSERTION LOG_PRIO_WARN
#define LOG_PRIO_DEFAULT_APPLICATION LOG_PRIO_ERROR

/* functions used internally that are otherwise useless */
void	__log_cap_cts(log_cat_t cat, char* wbuf, size_t len);
_Bool   __log_slt_is_std(log_slt_key_t key);
_Bool   __log_slt_is_valid(log_slt_key_t key);
void	__log_fm_flip_bit(log_slt_key_t target);
void	__log_fm_zero_bit(log_slt_key_t target);
void	__log_fm_one_bit(log_slt_key_t target);

/* internal state machine, stack allocated */
static log_state_t smachine = { 
	.files={ 0 }, 
	.flag=0xFF, 
	.callback=log_fc_default, 
	.capp = LOG_PRIO_DEFAULT_APPLICATION,
	.cassert = LOG_PRIO_DEFAULT_ASSERTION,
	.ctest = LOG_PRIO_DEFAULT_TEST,
	.tail = { NULL },
};

static const char* cat_lut[LOG_CAT_RESERVED0] = { "APPLICATION","ASSERTION","TEST","ERROR","SYS","RENDER", };
static const char* prio_lut[LOG_PRIO_COUNT] = { NULL,"TRACE","DEBUG","INFO","WARN","ERROR","FATAL" };


/* core */

void log_message(log_cat_t c, log_prio_t p, int ln, char* file, char* func, const char* fmt, ...) {
	if(!smachine.callback || !fmt)
		return;
	if (p < log_cap_get(c))
		return;

	va_list args;
	va_start(args, fmt);
	
	char out[LOG_SLT_BUFF_SIZE], post[LOG_SLT_BUFF_SIZE];
	vsnprintf(out, LOG_SLT_BUFF_SIZE, fmt, args);
	
	char* fchar = out + (strnlen(out, LOG_SLT_BUFF_SIZE)-1);
	if (fchar != out)
		if (*fchar == '\n' || *fchar == '\r') 
			*fchar = '\0';
	
	(*smachine.callback)(out, post, LOG_SLT_BUFF_SIZE, c, p, ln, file, func);

	log_slt_key_t it = 0, flag = smachine.flag;
	while (flag) {
		if ((flag & 1) && smachine.files[it])
			fputs(post, smachine.files[it]);

		flag >>= 1;
		it++;
	}

	va_end(args);
}

void log_quit(void) {
	log_cap_free();
	log_slt_close_all();

	free(smachine.tail);
	memset(&smachine, 0, sizeof(log_state_t));
}

void log_reset(void) {
	log_quit();

	smachine.tail = malloc(sizeof(log_cap_t));
	smachine.callback = &log_fc_default;

	smachine.cassert = LOG_PRIO_DEFAULT_ASSERTION;
	smachine.capp = LOG_PRIO_DEFAULT_APPLICATION;
	smachine.ctest = LOG_PRIO_DEFAULT_TEST;
}


/* formatting callback */

log_fc_f log_fc_get(void) { return smachine.callback; }

void log_fc_set(log_fc_f fc) { smachine.callback = fc; }

void log_fc_reset(void) { log_fc_set(log_fc_default); }

/* provided purely as an example */
void log_fc_default(const char* i, char* o, size_t io_len, log_cat_t c, log_prio_t p, int ln, char* file, char* func) {
	char str_time[9];
	time_t t = time(NULL);
	strftime(str_time, sizeof(str_time), "%H:%M:%S", localtime(&t));

	char str_cat[LOG_CAT_NAME_BUFF];
	__log_cap_cts(c, str_cat, LOG_CAT_NAME_BUFF);

	sprintf_s(o, io_len, "%s | %-*s | %-6s | %s\n", str_time, LOG_CAT_NAME_BUFF, str_cat, prio_lut[p], i);
}


/* category associated priorities */

void log_cap_set_all(log_prio_t np) {
	for (log_cap_t*head = smachine.tail; head; head = head->next) {
		head->prio = np;
	}

	smachine.capp = np;
	smachine.ctest = np;
	smachine.cassert = np;
}

void log_cap_set(log_prio_t prio, log_cat_t cat) {
	if (cat == LOG_CAT_APPLICATION) { smachine.capp = cat; return; }
	if (cat == LOG_CAT_ASSERTION) { smachine.cassert = cat; return; }
	if (cat == LOG_CAT_TEST) { smachine.ctest = cat; return; }

	log_cap_t* head;
	for (head = smachine.tail; head; head = head->next) {

		if (head->cat == cat) {
			head->prio = prio;
			return;
		}
	}

	/* add new node */
	head = malloc(sizeof(log_cap_t));
	head->cat = cat;
	head->prio = prio;
	head->next = smachine.tail;
	smachine.tail = head;
}

log_prio_t log_cap_get(log_cat_t cat) {
	if (cat == LOG_CAT_APPLICATION) return smachine.capp;
	if (cat == LOG_CAT_ASSERTION) return smachine.cassert;
	if (cat == LOG_CAT_TEST) return smachine.ctest;

	for (log_cap_t* head = smachine.tail; head; head = head->next) {
		if (head->cat == cat)
			return head->prio;
	}
	
	return LOG_PRIO_DEFAULT;
}

void log_cap_free(void) {
	smachine.cassert = LOG_PRIO_DEFAULT_ASSERTION;
	smachine.capp = LOG_PRIO_DEFAULT_APPLICATION;
	smachine.ctest = LOG_PRIO_DEFAULT_TEST;
	
	log_cap_t* head;
	while (smachine.tail) {
		head = smachine.tail;
		smachine.tail = smachine.tail->next;

		free(head);
		head = NULL;
	}
	smachine.tail = NULL;
}

void __log_cap_cts(log_cat_t cat, char* wbuf, size_t len) {
	assert(cat >= 0 && cat < LOG_CAT_COUNT);
	if (cat < LOG_CAT_RESERVED0) {
		strncpy(wbuf, cat_lut[cat], len);
		return;
	}

	if (cat > LOG_CAT_RESERVED7) sprintf_s(wbuf, len, "CUSTOM%i", cat - LOG_CAT_CUSTOM0);
	else /*					  */ sprintf_s(wbuf, len, "RESERVED%i", cat - LOG_CAT_RESERVED0);
}


/* sequential log targets */

errno_t log_slt_open(FILE* fp, log_slt_key_t* out) {
	log_slt_key_t it;
	for (it = LOG_SLT0; it < LOG_SLT_COUNT; it++) {
		if (!smachine.files[it]) {
			if (out != NULL)
				*out = it;

			smachine.files[it] = fp;
			return LOG_ENONE;
		}
	}

	return LOG_EFULL_TARGETS;
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

	for (it = LOG_SLT0; it < LOG_SLT_COUNT; it++) {
		if (__log_slt_is_valid(it))
			err |= fclose(smachine.files[it]);

		smachine.files[it] = NULL;
	}

	return (!err)?LOG_ENONE:LOG_EBAD_CLOSE;
}

FILE* log_slt_get(log_slt_key_t key) { return smachine.files[key]; }


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