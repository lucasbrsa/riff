#include "mat.h"

mat* mat_init(unsigned int width, unsigned int height) {
	//mat m = { calloc((width * height), sizeof(float)), width, height };
	mat* m = malloc(sizeof(mat));
	(*m) = (struct mat_t) { .matrix = calloc((width * height), sizeof(float)), .width = width, .height = height };
	return m;
}

mat mat_mult(mat* a, mat* b) {

}
