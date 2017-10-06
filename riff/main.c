#include "vector.h"
#include "hashmap.h"
#include "log.h"
#include "str.h"

#include <string.h>
#include <stdlib.h>
#include <assert.h>

static char __gbuf[128];

#define tdeclare(name) \
	const char* name(void)

#define tsuccess() \
	return NULL;

#define terror(m) \
	return m

#define tassert(m) \
	do { if (!(m)) return \
		snprintf(__gbuf, sizeof(__gbuf), "Assertion failed %s:%u  - %s", \
		__FILE__, __LINE__, #m), __gbuf; } while(0)

char* __tassert(const char* message, const char* file, unsigned line) {
	return __gbuf;
}

#define tassertstreq(a, b) \
	tassert(strcmp((a), (b)) == 0)

#define test(func) \
	do { printf("running test %s:\n", #func); const char* k = func(); \
	if (k) printf("failed: %s\n", k); else printf("success\n"); } while(0)

tdeclare(vector_test);
tdeclare(hashmap_test);
tdeclare(log_test);
tdeclare(str_test);

int main() {
	test(vector_test);
	test(hashmap_test);
	test(log_test);
	test(str_test);

	return 0;
}

typedef struct {
	int radius, mass, x, y;
} vec_example_t;

tdeclare(vector_test) {
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

	tassert(k);
	tassert(vector_eq(k, m));

	vector_clear(m);

	tassert(vector_empty(m));

	vector_free(m);

	vector_t* j = vector_init(10, sizeof(char*), NULL);
	vector_push_back(j, "pranked");
	vector_swap(j, k);

	tassertstreq(vector_att(k, 0, char*), "pranked");

	char* buffer = malloc(sizeof("I'd just like to interject for a moment... ") + 1);
	char* b = buffer;

	for (vector_iterator(j, char*, it)) {
		size_t l = strlen(*it);
		memcpy(b, *it, l);
		b += l;
		*b++ = ' ';
	}

	tassertstreq(buffer, "I'd just like to interject for a moment... ");
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

	tassert((*(vec_example_t**)vector_at(heap_vec, 1))->mass == 75);

	vector_remove(heap_vec, 1);
	vector_free(heap_vec);

	tsuccess();
}

void iscool(hashmap_bucket_t* b, void* v) {
	if (strcmp(b->key, "cameron") == 0)
		str_cpy(v, b->value);
}

tdeclare(hashmap_test) {
	/* realistically a table of length 2 is absurd, but this is to maximize collisions */
	hashmap_t* hm = hashmap_init(2, NULL);

	tassert(hashmap_empty(hm));
	hashmap_set(hm, "james", "is a sick cunt");
	hashmap_set(hm, "john", "is just a cunt");
	hashmap_set(hm, "james", "is a great bloke");
	hashmap_remove(hm, "john");
	tassert(hashmap_length(hm) == 1);

	tassertstreq(hashmap_get(hm, "james"), "is a great bloke");
	tassert(hashmap_get(hm, "peter") == NULL);

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
	tassertstreq(buffer, "is a god");

	hashmap_set(hm, "cameron", "is an idiot");
	hashmap_set(hm, "ruchir", "is a god");

	hashmap_remove(hm, "james");
	hashmap_remove(hm, "jim");
	hashmap_remove(hm, "jerry");
	hashmap_remove(hm, "rick");
	hashmap_remove(hm, "beth");
	hashmap_remove(hm, "summer");
	hashmap_remove(hm, "morty");

	tassert(hashmap_get(hm, "rick") == NULL);
	tassert(hashmap_get(hm, "beth") == NULL);

	hashmap_free(hm);

	tsuccess();
}

tdeclare(log_test) {
	/* rules will be handled later */
	log_rule_t* r = malloc(sizeof(log_rule_t));
	r->rule = __handle_rule_stdout;
	r->ruleimpl_ptr = NULL;

	log_logger_t* my_logger = log_logger_init("test logger", r);

	// tassert(my_logger != NULL);
	// tassert(my_logger == log_logger_get("test logger"));

	// log_logger_get("test logger");

	log_log(my_logger, "We have liftoff.");

	log_free();
	free(r);

	tsuccess();
}

bool apply_filter(char c) {
	return !!((int) c % 2);
}

tdeclare(str_test) {
	char* buffer = malloc(256);
	tassertstreq("-1238948", str_its(-1238948, buffer));

	str_cpy(buffer, "\n  meme man \n  ");
	tassertstreq(str_strip(buffer), "meme man");

	str_ttble_t* tt = str_gen_ttble(
			"abcdefghijklmnopqrstuvwxyz",
			"cdefghijklmnopqrstuvwxyzab");

	tassertstreq(str_trans_cpy(tt, buffer, "hello world!"), "jgnnq yqtnf!");
	str_free_ttble(tt);

	tassertstreq(str_filter(buffer, "hello world!", apply_filter), "eowo!");

	tassert(str_find_substr("meme i am a meme man lordmeme", "meme") == 0);
	tassert(str_find_substr_count("meme i am a meme man lordmeme", "meme") == 3);

	char* a = str_replace(buffer, "meme i am a meme man lordmeme", "meme", "gay");
	tassertstreq(a, "gay i am a gay man lordgay");

	tassert(str_searcho("abcdefghijklmnopqrstuvwxyz", 0, 26, 'n') == 13);
	tassert(str_search("abcdefghijklmnopqrstuvwxyz", 'n') == 13);

	str_cpy(buffer, "abcdefghijklmnopqrstuvwxyz");
	tassert(str_dirsearch(buffer, buffer + 26, 'n') == 13);

	tassert(*(str_find_chr(buffer, 'm') + 1) == 'n');

	str_cpy(buffer, "I'd like to interject for a moment");
	str_tolower(buffer);

	tassert(str_find_first(buffer, "aeiou") == 0);
	tassert(str_find_last(buffer, "aeiou") == 31);
	tassert(str_find_firstn(buffer, "aeiou") == 1);
	tassert(str_find_lastn(buffer, "aeiou") == 33);

	str_cpy(buffer, "I am a God!");
	tassertstreq(str_swap_case(buffer), "i AM A gOD!");
	tassertstreq(str_tolower(buffer), "i am a god!");
	tassertstreq(str_toupper(buffer), "I AM A GOD!");

	tassertstreq(str_dircpy(buffer, "hello world!", -1, -2), "!lo le");
	tassertstreq(str_dircpy(buffer, "hello world!", -1, 2), "hlowrd");

	tassertstreq(str_concat(4, buffer, "I ", "am ", "a ", "God!"), "I am a God!");

	tassertstreq(str_mul(buffer, "meme ", 3), "meme meme meme ");

	char* nbuffer = str_dup(buffer);
	tassertstreq(buffer, nbuffer);
	free(nbuffer);

	tassertstreq(str_lpad(buffer, "example", 11, '*'), "****example");
	tassertstreq(str_rpad(buffer, "example", 11, '*'), "example****");
	tassertstreq(str_cpad(buffer, "example", 11, '*'), "**example**");

	tassertstreq(str_tab_to_space(buffer, "this \t  \t", 4), "this           ");
	tassertstreq(str_space_to_tab(buffer, "this           ", 4), "this\t\t   ");

	tassertstreq(str_cescape('\f'), "\\f");
	tassertstreq(str_escape(buffer, "Hello \t world!"), "Hello \\t world!");

	/* adress str_split linker error */
	str_stble_t st = str_split("I am a God!", ' ');

	tassertstreq(st.tble[0], "I");
	tassertstreq(st.tble[1], "am");
	tassertstreq(st.tble[2], "a");
	tassertstreq(st.tble[3], "God!");

	tassertstreq(str_join(buffer, '*', st), "I*am*a*God!");

	str_free_split(st);

	tassert(str_len_including("hello world!", "aeiou") == 3);
	tassert(str_len_excluding("hello world!", "aeiou") == 9);

	tassertstreq(str_zfill(buffer, "1234", 8), "00001234");

	tassertstreq(str_reverse(buffer, "I am a God!"), "!doG a ma I");

	tassertstreq(str_newline_to_space(buffer, "Hello\nworld!"), "Hello world!");

	tassertstreq(str_remove(buffer, "Hello world!", "Hello "), "world!");

	char* c = str_dup("meme");
	for (str_iterator(c, it))
		c[it.i] = it.c + 1;

	tassertstreq(c, "nfnf");

	free(buffer);

	tsuccess();
}
