#include "vector.h"
#include "hashmap.h"
#include "log.h"
#include "str.h"

#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define tdeclare(name) \
	const char* name(void)

#define tsuccess() \
	return NULL;

#define terror(m) \
	return m

#define tassert(m) \
	do { if (!(m)) return #m; } while(0)

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

	tsuccess();
}

tdeclare(hashmap_test) {
	hashmap_t* hm = hashmap_init(4, NULL);

	tassert(hashmap_empty(hm));
	hashmap_insert(hm, "james", "is a sick cunt");
	hashmap_insert(hm, "john", "is just a cunt");
	hashmap_insert(hm, "james", "is a great bloke");
	tassert(!hashmap_empty(hm));

	tassertstreq((char*)hashmap_at(hm, "john"), "is just a cunt");
	tassertstreq((char*)hashmap_at(hm, "james"), "is a great bloke");
	tassert(hashmap_at(hm, "peter") == NULL);

	hashmap_iterator_t it = hashmap_front(hm);

	bool a = *(char**)it.value == (char*)hashmap_at(hm, "john") ;
	bool b = *(char**)it.value == (char*)hashmap_at(hm, "james") ;
	tassert(a || b);

	/* iterators aren't working yet */
	/* it = hashmap_next(hm, it); */
	/* hashmap_iterator_t mit = hashmap_back(hm); */
	/* tassert(hashmap_iterator_eq(mit, it)); */

	hashmap_free(hm);

	tsuccess();
}

tdeclare(log_test) {
	log_logger_t* my_logger = log_logger_init("test logger", NULL);

	tassert(my_logger != NULL);
	tassert(my_logger == log_logger_get("test logger"));

	log_logger_free(my_logger);
	// log_free(); <-- not working

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

	/* iterators could do with more tests... */
	char* c = str_dup("meme");

	for (str_iter(c, it))
		c[it.i] = it.c + 1;

	tassertstreq(c, "nfnf");

	free(buffer);

	tsuccess();
}
