#ifndef MAT_H
#define MAT_H

#include <stdio.h>
#include <stdbool.h>
#include "generic.h"

/* represents an n * m matrix */
typedef struct mat_t {
	
	/* holds the matrix, is calloced so will need to be freed */
	float** matrix;

	/* number of rows */
	unsigned int width;

	/* number of columns */
	unsigned int height;

	/* stores the transpose of this matrix
	 * so it isn't calculated needlessly
	 * is updated when transpose() is called
	*/
	float** transpose;
} mat;

/* create and return a new n * m sized matrix
 * params for size of matrix */
mat mat_init(const unsigned int width, const unsigned int height);

/* free the memory used by a matrix */
void mat_free(mat* m);

/* grow the matrix to a new size */
bool mat_resize(mat* m, const unsigned int new_width, const unsigned int new_height);

/* set all elements to 0 */
bool mat_reset(mat* m);

#define mat_width(m) \
	m.width

#define mat_height(m) \
	m.height

#define mat_size(m) \
	(m.width * m.height)

//#define mat_print(m) \

#endif
