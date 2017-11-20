#include "test.h"
#include "str.h"

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
	free(c);
}
