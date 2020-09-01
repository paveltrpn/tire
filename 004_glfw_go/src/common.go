package main

import "math"

/* math constants from libc math.h */

const (
	f_eps      float32 = 5.96e-08
	M_E        float32 = 2.718281828459045235360287471352662498 /* e */
	M_LOG2E    float32 = 1.442695040888963407359924681001892137 /* log_2 e */
	M_LOG10E   float32 = 0.434294481903251827651128918916605082 /* log_10 e */
	M_LN2      float32 = 0.693147180559945309417232121458176568 /* log_e 2 */
	M_LN10     float32 = 2.302585092994045684017991454684364208 /* log_e 10 */
	M_PI       float32 = 3.141592653589793238462643383279502884 /* pi */
	M_PI_2     float32 = 1.570796326794896619231321691639751442 /* pi/2 */
	M_PI_4     float32 = 0.785398163397448309615660845819875721 /* pi/4 */
	M_1_PI     float32 = 0.318309886183790671537767526745028724 /* 1/pi */
	M_2_PI     float32 = 0.636619772367581343075535053490057448 /* 2/pi */
	M_2_SQRTPI float32 = 1.128379167095512573896158903121545172 /* 2/sqrt(pi) */
	M_SQRT2    float32 = 1.414213562373095048801688724209698079 /* sqrt(2) */
	M_SQRT1_2  float32 = 0.707106781186547524400844362104849039 /* 1/sqrt(2) */
)

const (
	_XC int8 = 0
	_YC int8 = 1
	_ZC int8 = 2
	_WC int8 = 3
)

/*
	Function prototypes

func id_rw(i int32, j int32, n int32) int32
func id_cw(i int32, j int32, n int32) int32
func fabs(x float32) float32
func deg_to_rad(deg float32) float32
func cosf(x float32) float32
func sinf(x float32) float32
func sqrtf(x float32) float32
*/

/*	multidimensional array mapping, array[i][j]
	row-wise (C, C++):
	(0	1)
	(2	3)

	column-wise (Fortran, Matlab):
	(0	2)
	(1	3)
*/

func idRw(i int32, j int32, n int32) int32 {
	return (i*n + j)
}

func idCw(i int32, j int32, n int32) int32 {
	return (j*n + i)
}

func fabs(x float32) float32 {
	if x < 0.0 {
		return -x
	} else {
		return x
	}
}

func degToRad(deg float32) float32 {
	return deg * M_PI / 180.0
}

/*
func cosf(x float32) float32 {
	if x < 0.0 {
		x = -x
	}

	for M_PI < x {
		x -= M_2_PI
	}

	return 1.0 - (x*x/2.0)*(1.0-(x*x/12.0)*(1.0-(x*x/30.0)*(1.0-x*x/56.0)))
}

func sinf(x float32) float32 {
	return cosf(x - M_PI_2)
}
*/

func cosf(x float32) float32 {
	return float32(math.Cos(float64(x)))
}

func sinf(x float32) float32 {
	return float32(math.Sin(float64(x)))
}

func sqrtf(x float32) float32 {
	return float32(math.Sqrt(float64(x)))
}
