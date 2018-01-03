#ifndef MAT_H
#define MAT_H

#include <stdio.h>
#include <stdbool.h>
#include "generic.h"

/* represents an n * m matrix */
typedef struct mat_t {
	
	/* holds the matrix, is calloced so it'll need to be freed */
	float* matrix;

	/* number of rows */
	unsigned int width;

	/* number of columns */
	unsigned int height;

} mat;

/* create and return a new n * m sized matrix */
mat* mat_init(const unsigned int width, const unsigned int height);

/* grow the matrix to a new size */
bool mat_resize(mat* m, const unsigned int new_width, const unsigned int new_height);

/* set all elements to 0 */
bool mat_reset(mat* m);

mat mat_mult(mat* a, mat* b);

#define mat_free(mp) \
	free(mp->matrix)

#define mat_width(m) \
	(m.width)

#define mat_height(m) \
	(m.height)

#define mat_size(m) \
	(m.width * m.height)

#define mat_print(m) \
	for(unsigned int y = 0; y < mat_height(m); y++) { \
		for(unsigned int x = 0; x < mat_width(m); x++) { \
			printf("%f ", m.matrix[y * mat_width(m) + y]); \
		} \
		putc('\n', stdout); \
	} \

#endif
