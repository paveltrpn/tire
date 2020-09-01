package main

import (
	"fmt"
)

type vec4 [4]float32

func vec4Show(v vec4) {
	fmt.Printf("%5.2f %5.2f %5.2f %5.2f\n", v[_XC], v[_YC], v[_ZC], v[_WC])
}

func vec4Copy(v vec4) (rt vec4) {
	rt[0] = v[0]
	rt[1] = v[1]
	rt[2] = v[2]
	rt[3] = v[3]

	return rt
}

func vec4Set(x, y, z, w float32) (rt vec4) {
	rt[0] = x
	rt[1] = y
	rt[2] = z
	rt[3] = w

	return rt
}

func vec4Lenght(v vec4) float32 {
	return sqrtf(v[_XC]*v[_XC] +
		v[_YC]*v[_YC] +
		v[_ZC]*v[_ZC] +
		v[_WC]*v[_WC])

}

func vec4Normalize(v vec4) (rt vec4) {
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

func vec4Scale(v vec4, scale float32) (rt vec4) {
	v[0] *= scale
	v[1] *= scale
	v[2] *= scale
	v[3] *= scale

	return rt
}

func vec4Invert(v vec4) (rt vec4) {
	rt[_XC] = -v[_XC]
	rt[_YC] = -v[_YC]
	rt[_ZC] = -v[_ZC]
	rt[_WC] = -v[_WC]

	return rt
}

func vec4Dot(a vec4, b vec4) float32 {
	return a[0]*b[0] + a[1]*b[1] + a[2]*b[2] + a[3]*b[3]
}

func vec4Sum(a, b vec4) (rt vec4) {
	rt[0] = a[0] + b[0]
	rt[1] = a[1] + b[1]
	rt[2] = a[2] + b[2]
	rt[3] = a[3] + b[3]

	return rt
}

func vec4Sub(a, b vec4) (rt vec4) {
	rt[0] = a[0] - b[0]
	rt[1] = a[1] - b[1]
	rt[2] = a[2] - b[2]
	rt[3] = a[3] - b[3]

	return rt
}
