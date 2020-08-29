package main

import (
	"fmt"
)

type vec2_t [2]float32

/* func proto

func vec2_show(v vec2_t)
func vec2_copy(v vec2_t) (rt vec2_t)
func vec2_set(x float32, y float32) (rt vec2_t)
func vec2_lenght(v vec2_t) float32
func vec2_normalize(v vec2_t)
func vec2_scale(v vec2_t, scale float32) (rt vec2_t)
func vec2_invert(v vec2_t) (rt vec2_t)
func vec2_dot(a vec2_t, b vec2_t) float32
func vec2_sum(a, b vec2_t) (rt vec2_t)
func vec2_sub(a, b vec2_t) (rt vec2_t)

*/

func vec2Show(v vec2_t) {
	fmt.Printf("%5.2f %5.2f\n", v[_XC], v[_YC])
}

func vec2Copy(v vec2_t) (rt vec2_t) {
	rt[0] = v[0]
	rt[1] = v[1]

	return rt
}

func vec2Set(x float32, y float32) (rt vec2_t) {
	rt[0] = x
	rt[1] = y

	return rt
}

func vec2Lenght(v vec2_t) float32 {
	return sqrtf(v[_XC]*v[_XC] +
		v[_YC]*v[_YC])

}

func vec2Normalize(v vec2_t) (rt vec2_t) {
	var (
		len float32
	)

	len = vec2Lenght(v)

	if len != 0.0 {
		rt[_XC] = v[_XC] / len
		rt[_YC] = v[_YC] / len
	}

	return rt
}

func vec2Scale(v vec2_t, scale float32) (rt vec2_t) {
	v[0] *= scale
	v[1] *= scale

	return rt
}

func vec2Invert(v vec2_t) (rt vec2_t) {
	rt[_XC] = -v[_XC]
	rt[_YC] = -v[_YC]

	return rt
}

func vec2Dot(a vec2_t, b vec2_t) float32 {
	return a[0]*b[0] + a[1]*b[1]
}

func vec2Sum(a, b vec2_t) (rt vec2_t) {
	rt[0] = a[0] + b[0]
	rt[1] = a[1] + b[1]

	return rt
}

func vec2Sub(a, b vec2_t) (rt vec2_t) {
	rt[0] = a[0] - b[0]
	rt[1] = a[1] - b[1]

	return rt
}
