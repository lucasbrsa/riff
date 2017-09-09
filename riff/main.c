#include <stdio.h>
#include "hashmap.h"

int main(int argc, char ** argv)
{

	hashmap h = hashmap_create(10);

	hashmap_set(&h, "name", "ruchir");

	puts(hashmap_get(&h, "name"));

	getchar(); return 0;
}
