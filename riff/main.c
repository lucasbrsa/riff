#include "generic.h"
#include "vector.h"

int main(int argc, char** argv) {
	vector_t* v = vector_init(8, sizeof(int), NULL);
	int a = 5;
	vector_push_back(v, &a);
	vector_print(v, int, "%d ");
	vector_free(v);
}
