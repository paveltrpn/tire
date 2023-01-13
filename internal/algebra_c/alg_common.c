
#include <stdint.h>

#include "algebra_c.h"

/*	multidimensional array mapping, array[i][j]
	row-wise (C, C++):
	(0	1)
	(2	3)

	column-wise (Fortran, Matlab):
	(0	2)
	(1	3)
*/

int32_t idRw(int32_t i, int32_t j, int32_t n) {
	return (i*n + j);
}

int32_t idCw(int32_t i, int32_t j, int32_t n) {
	return (j*n + i);
}
