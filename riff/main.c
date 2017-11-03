#define LOG_LEVEL 3

#include "test.h"

#include "vector.h"
#include "error.h"
#include "hashmap.h"
#include "log.h"
#include "str.h"
#include "os.h"

#include <string.h>
#include <stdlib.h>
#include <assert.h>

typedef struct {
	int radius, mass, x, y;
} vec_example_t;

void vector_test(void) {
	vector_t* m = vector_init(8, sizeof(char*), NULL);

	char* appends[] = { "to", "interject", "for", "a" };

	vector_push_back(m, "Richard");
	vector_push_back(m, "like");
	vector_insert(m, 1, "just");
	vector_insert(m, 1, "would");

	vector_append(m, appends, sizeof(appends) / sizeof(*appends));

	vector_reserve(m, 2); /* will do nothing */
	vector_reserve(m, 128);

	vector_push_back(m, "second");
	vector_remove(m, 1);
	vector_set(m, 0, "I'd");
	vector_pop_back(m);
	vector_push_back(m, "moment...");

	vector_t* k = vector_copy(m);

	test_assert(k);
	test_assert(vector_eq(k, m));

	vector_clear(m);

	test_assert(vector_empty(m));

	vector_free(m);

	vector_t* j = vector_init(10, sizeof(char*), NULL);
	vector_push_back(j, "pranked");
	vector_swap(j, k);

	test_seq(vector_att(k, 0, char*), "pranked");

	char* buffer = malloc(sizeof("I'd just like to interject for a moment... ") + 1);
	char* b = buffer;

	for (vector_iterator(j, char*, it)) {
		size_t l = strlen(*it);
		memcpy(b, *it, l);
		b += l;
		*b++ = ' ';
	}

	test_seq(buffer, "I'd just like to interject for a moment... ");
	free(buffer);

	vector_free(j);

	vector_t* heap_vec = vector_init(32, sizeof(vec_example_t*), free);

	vec_example_t* mexa = malloc(sizeof(vec_example_t));
	mexa->mass = 12; mexa->radius = 1; mexa->x = 0; mexa->y = 0;

	vec_example_t* mexb = malloc(sizeof(vec_example_t));
	mexb->mass = 75; mexb->radius = 28; mexb->x = 3; mexa->y = 13;

	vec_example_t* mexc = malloc(sizeof(vec_example_t));
	mexc->mass = 3; mexc->radius = 1; mexc->x = -32; mexc->y = 5;

	vector_push_back(heap_vec, mexa);
	vector_push_back(heap_vec, mexb);
	vector_push_back(heap_vec, mexc);

	test_assert((*(vec_example_t**)vector_at(heap_vec, 1))->mass == 75);

	vector_remove(heap_vec, 1);
	vector_free(heap_vec);
}

void iscool(hashmap_bucket_t* b, void* v) {
	if (strcmp(b->key, "cameron") == 0)
		str_cpy(v, b->value);
}

void hashmap_test(void) {
	/* realistically a table of length 2 is absurd, but this is to maximize collisions */
	hashmap_t* hm = hashmap_init(2, NULL);

	test_assert(hashmap_empty(hm));
	hashmap_set(hm, "james", "is a sick cunt");
	hashmap_set(hm, "john", "is just a cunt");
	hashmap_set(hm, "james", "is a great bloke");
	hashmap_remove(hm, "john");
	test_assert(hashmap_length(hm) == 1);

	test_seq(hashmap_get(hm, "james"), "is a great bloke");
	test_assert(hashmap_get(hm, "peter") == NULL);

	hashmap_set(hm, "jim", "is lame");
	hashmap_set(hm, "jerry", "is stupid");
	hashmap_set(hm, "rick", "is smart");
	hashmap_set(hm, "beth", "is annoying");
	hashmap_set(hm, "summer", "is eh");
	hashmap_set(hm, "morty", "is named after an old jewish comedian");
	hashmap_set(hm, "cameron", "is a god");
	hashmap_set(hm, "ruchir", "is a demigod");

	char* buffer = malloc(128);
	hashmap_iterate(hm, iscool, buffer);
	test_seq(buffer, "is a god");

	hashmap_set(hm, "cameron", "is an idiot");
	hashmap_set(hm, "ruchir", "is a god");

	hashmap_remove(hm, "james");
	hashmap_remove(hm, "jim");
	hashmap_remove(hm, "jerry");
	hashmap_remove(hm, "rick");
	hashmap_remove(hm, "beth");
	hashmap_remove(hm, "summer");
	hashmap_remove(hm, "morty");

	test_assert(hashmap_get(hm, "rick") == NULL);
	test_assert(hashmap_get(hm, "beth") == NULL);

	hashmap_free(hm);

	hashmap_t* memhm = hashmap_init(8, free);

	int* ayy = malloc(sizeof(int));
	int* bae = malloc(sizeof(int));

	*ayy = 69;
	*bae = 420;

	hashmap_set(memhm, "ayy", ayy);
	hashmap_set(memhm, "bae", bae);

	*bae = 720;

	test_assert(*(int*)hashmap_get(memhm, "bae") == 720);

	hashmap_free(memhm);
}

void log_test(void) {
	return;
	log_logger_t* console = log_logger("console", log_writer_stdout());

	log_notice(console, "Hello %s", "world!");
	log_notice(console, "I am a simple stdout logger");

	log_warn_if(1 == 0, console, "Oh no...");
	log_info(log_logger_get("console"), "You can find a logger by name.");

	log_panic(console, "I'm suiciding now...");
	log_logger_free(console);

	log_logger_t* rainbow = log_logger("rainbow", log_writer_stdout_coloured());

	log_fmt_set("[%t] [%n] [%p] %m");

	log_debug(rainbow, "hello");
	log_info(rainbow, "hello");
	log_notice(rainbow, "hello");
	log_warn(rainbow, "hello");
	log_error(rainbow, "hello");
	log_panic(rainbow, "hello");

	log_logger_t* syslogger = log_logger("syslogger", log_writer_syslog(0));
	log_notice(syslogger, "This was written through %s", "log.h");


	LOG_DEBUG(rainbow, "This line of code never ran...");
	LOG_INFO(rainbow, "This line of code never ran...");
	LOG_NOTICE(rainbow, "This line of code ran...");

	log_free();

	log_logger_t* general_logger = log_logger("general", log_writer_null());
	log_logger_add_writer(general_logger, log_writer_stdout_coloured());
	log_logger_add_writer(general_logger, log_writer_syslog(0));
	log_logger_add_writer(general_logger, log_writer_file("../res/general.log"));

	log_notice(log_logger_get("general"), "This was created by a multi-writer logger");
}

bool apply_filter(char c) {
	return !!((int) c % 2);
}

void str_test(void) {
	char* mem = str_sprintf("I am a %s, who is %d years old.", "god", 15);
	test_seq("I am a god, who is 15 years old.", mem);
	free(mem);

	char* buffer = malloc(256);
	test_seq("-1238948", str_its(-1238948, buffer));

	str_cpy(buffer, "\n  meme man \n  ");
	test_seq(str_strip(buffer), "meme man");

	str_ttble_t* tt = str_gen_ttble(
			"abcdefghijklmnopqrstuvwxyz",
			"cdefghijklmnopqrstuvwxyzab");

	test_seq(str_trans_cpy(tt, buffer, "hello world!"), "jgnnq yqtnf!");
	str_free_ttble(tt);

	test_seq(str_filter(buffer, "hello world!", apply_filter), "eowo!");

	test_assert(str_find_substr("meme i am a meme man lordmeme", "meme") == 0);
	test_assert(str_find_substr_count("meme i am a meme man lordmeme", "meme") == 3);

	char* a = str_replace(buffer, "meme i am a meme man lordmeme", "meme", "gay");
	test_seq(a, "gay i am a gay man lordgay");

	test_assert(str_searcho("abcdefghijklmnopqrstuvwxyz", 0, 26, 'n') == 13);
	test_assert(str_search("abcdefghijklmnopqrstuvwxyz", 'n') == 13);

	str_cpy(buffer, "abcdefghijklmnopqrstuvwxyz");
	test_assert(str_dirsearch(buffer, buffer + 26, 'n') == 13);

	test_assert(*(str_find_chr(buffer, 'm') + 1) == 'n');

	str_cpy(buffer, "I'd like to interject for a moment");
	str_tolower(buffer);

	test_assert(str_find_first(buffer, "aeiou") == 0);
	test_assert(str_find_last(buffer, "aeiou") == 31);
	test_assert(str_find_firstn(buffer, "aeiou") == 1);
	test_assert(str_find_lastn(buffer, "aeiou") == 33);

	str_cpy(buffer, "I am a God!");
	test_seq(str_swap_case(buffer), "i AM A gOD!");
	test_seq(str_tolower(buffer), "i am a god!");
	test_seq(str_toupper(buffer), "I AM A GOD!");

	test_seq(str_dircpy(buffer, "hello world!", -1, -2), "!lo le");
	test_seq(str_dircpy(buffer, "hello world!", -1, 2), "hlowrd");

	test_seq(str_concat(4, buffer, "I ", "am ", "a ", "God!"), "I am a God!");

	test_seq(str_mul(buffer, "meme ", 3), "meme meme meme ");

	char* nbuffer = str_dup(buffer);
	test_seq(buffer, nbuffer);
	free(nbuffer);

	test_seq(str_lpad(buffer, "example", 11, '*'), "****example");
	test_seq(str_rpad(buffer, "example", 11, '*'), "example****");
	test_seq(str_cpad(buffer, "example", 11, '*'), "**example**");

	test_seq(str_tab_to_space(buffer, "this \t  \t", 4), "this           ");
	test_seq(str_space_to_tab(buffer, "this           ", 4), "this\t\t   ");

	test_seq(str_cescape('\f'), "\\f");
	test_seq(str_escape(buffer, "Hello \t world!"), "Hello \\t world!");

	/* adress str_split linker error */
	str_stble_t st = str_split("I am a God!", ' ');

	test_seq(st.tble[0], "I");
	test_seq(st.tble[1], "am");
	test_seq(st.tble[2], "a");
	test_seq(st.tble[3], "God!");

	test_seq(str_join(buffer, '*', st), "I*am*a*God!");

	str_free_split(st);

	test_assert(str_len_including("hello world!", "aeiou") == 3);
	test_assert(str_len_excluding("hello world!", "aeiou") == 9);

	test_seq(str_zfill(buffer, "1234", 8), "00001234");

	test_seq(str_reverse(buffer, "I am a God!"), "!doG a ma I");

	test_seq(str_newline_to_space(buffer, "Hello\nworld!"), "Hello world!");

	test_seq(str_remove(buffer, "Hello world!", "Hello "), "world!");

	char* c = str_dup("meme");
	for (str_iterator(c, it))
		c[it.i] = it.c + 1;

	test_seq(c, "nfnf");

	free(buffer);
}

void os_test(void) {
	OS_MKDIR("some_dir");
	OS_RENAME("some_dir", "my_dir");
	OS_RMDIR("my_dir");
}

void error_test(void) {
	test_seq(error_gets(), "no error");
	error_set("something's wrong...");

	test_seq(error_gets(), "something's wrong...");
	error_code(ERROR_LOGICAL);

	test_seq(error_gets(), "logical");
	error_clear();
}

int main() {
	test_run(vector_test);
	test_run(hashmap_test);
	test_run(log_test);
	test_run(str_test);
	test_run(os_test);
	test_run(error_test);

	test_resolve();
}
