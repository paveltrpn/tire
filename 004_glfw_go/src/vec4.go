package main

import (
	"fmt"
)

type vec4_t [4]float32

/* func proto

func vec4_show(v vec4_t)
func vec4_copy(v vec4_t) (rt vec4_t)
func vec4_set(x float32, y float32) (rt vec4_t)
func vec4_lenght(v vec4_t) float32
func vec4_normalize(v vec4_t)
func vec4_scale(v vec4_t, scale float32) (rt vec4_t)
func vec4_invert(v vec4_t) (rt vec4_t)
func vec4_dot(a vec4_t, b vec4_t) float32
func vec4_sum(a, b vec4_t) (rt vec4_t)
func vec4_sub(a, b vec4_t) (rt vec4_t)

*/

func vec4Show(v vec4_t) {
	fmt.Printf("%5.2f %5.2f %5.2f %5.2f\n", v[_XC], v[_YC], v[_ZC], v[_WC])
}

func vec4Copy(v vec4_t) (rt vec4_t) {
	rt[0] = v[0]
	rt[1] = v[1]
	rt[2] = v[2]
	rt[3] = v[3]

	return rt
}

func vec4Set(x, y, z, w float32) (rt vec4_t) {
	rt[0] = x
	rt[1] = y
	rt[2] = z
	rt[3] = w

	return rt
}

func vec4Lenght(v vec4_t) float32 {
	return sqrtf(v[_XC]*v[_XC] +
		v[_YC]*v[_YC] +
		v[_ZC]*v[_ZC] +
		v[_WC]*v[_WC])

}

func vec4Normalize(v vec4_t) (rt vec4_t) {
	var (
		len float32
	)

	len = vec4Lenght(v)

	if len != 0.0 {
		rt[_XC] = v[_XC] / len
		rt[_YC] = v[_YC] / len
		rt[_ZC] = v[_ZC] / len
		rt[_WC] = v[_WC] / len
	}

	return rt
}

func vec4Scale(v vec4_t, scale float32) (rt vec4_t) {
	v[0] *= scale
	v[1] *= scale
	v[2] *= scale
	v[3] *= scale

	return rt
}

func vec4Invert(v vec4_t) (rt vec4_t) {
	rt[_XC] = -v[_XC]
	rt[_YC] = -v[_YC]
	rt[_ZC] = -v[_ZC]
	rt[_WC] = -v[_WC]

	return rt
}

func vec4Dot(a vec4_t, b vec4_t) float32 {
	return a[0]*b[0] + a[1]*b[1] + a[2]*b[2] + a[3]*b[3]
}

func vec4Sum(a, b vec4_t) (rt vec4_t) {
	rt[0] = a[0] + b[0]
	rt[1] = a[1] + b[1]
	rt[2] = a[2] + b[2]
	rt[3] = a[3] + b[3]

	return rt
}

func vec4Sub(a, b vec4_t) (rt vec4_t) {
	rt[0] = a[0] - b[0]
	rt[1] = a[1] - b[1]
	rt[2] = a[2] - b[2]
	rt[3] = a[3] - b[3]

	return rt
}
