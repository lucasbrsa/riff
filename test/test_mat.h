#include "mat.h"
#include "test.h"

#define WIDTH  4
#define HEIGHT 3

void mat_test(void) {
	mat* m = mat_init(WIDTH, HEIGHT);
	for(int i = 0; i < HEIGHT; i++) {
		for(int j = 0; j < WIDTH; j++) {
			printf("%f ", m->matrix[i * WIDTH + j]);
		}
		putc('\n', stdout);
	}
	free(m->matrix);
}
