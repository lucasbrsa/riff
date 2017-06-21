#include <stdio.h>
#include "hashmap.h"

int main(int argc, char ** argv)
{
	hashmap h = hashmap_create();


	hashmap_pair p = hashmap_pair_create("Larissa", "iis cool!!!");
	set(&h, p);


	printf("\n\nKEY OUTPUTED: %s\n\n", get(&h, "larissa"));

	getchar(); return 0;
}