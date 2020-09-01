
#pragma once

#include <cmath>
#include <tuple>

enum {_XC, _YC, _ZC, _WC};
enum mtrx_type {MTRX_IDTT, MTRX_ZERO};

#define M_PI 3.14159265358979323846

constexpr float f_eps = 5.96e-08;
constexpr float deg_to_rad(float deg) {
	return deg * M_PI/180.0f;
}

/*	multidimensional array mapping, array[i][j]
	row-wise (C, C++):
	(0	1)
	(2	3)

	column-wise (Fortran, Matlab):
	(0	2)
	(1	3)
*/

constexpr int32_t id_rw(int32_t i, int32_t j, int32_t n) {
	return (i*n + j);
};

constexpr int32_t id_cw(int32_t i, int32_t j, int32_t n) {
	return (j*n + i);
};
