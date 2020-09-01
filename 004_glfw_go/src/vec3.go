package main

import (
	"fmt"
)

type vec3 [3]float32

func vec3Show(v vec3) {
	fmt.Printf("%5.2f %5.2f %5.2f\n", v[_XC], v[_YC], v[_ZC])
}

func vec3Copy(v vec3) (rt vec3) {
	rt[0] = v[0]
	rt[1] = v[1]
	rt[2] = v[2]

	return rt
}

func vec3Set(x float32, y float32, z float32) (rt vec3) {
	rt[0] = x
	rt[1] = y
	rt[2] = z

	return rt
}

func vec3Lenght(v vec3) float32 {
	return sqrtf(v[_XC]*v[_XC] +
		v[_YC]*v[_YC] +
		v[_ZC]*v[_ZC])

}

func vec3Normalize(v vec3) (rt vec3) {
	var (
		len float32
	)

	len = vec3Lenght(v)

	if len != 0.0 {
		rt[_ZC] = v[_ZC] / len
		rt[_XC] = v[_XC] / len
		rt[_YC] = v[_YC] / len
	}

	return rt
}

func vec3Scale(v vec3, scale float32) (rt vec3) {
	v[0] *= scale
	v[1] *= scale
	v[2] *= scale

	return rt
}

func vec3Invert(v vec3) (rt vec3) {
	rt[_XC] = -v[_XC]
	rt[_YC] = -v[_YC]
	rt[_ZC] = -v[_ZC]

	return rt
}

func vec3Dot(a vec3, b vec3) float32 {
	return a[0]*b[0] + a[1]*b[1] + a[2]*b[2]
}

func vec3Sum(a, b vec3) (rt vec3) {
	rt[0] = a[0] + b[0]
	rt[1] = a[1] + b[1]
	rt[2] = a[2] + b[2]

	return rt
}

func vec3Sub(a, b vec3) (rt vec3) {
	rt[0] = a[0] - b[0]
	rt[1] = a[1] - b[1]
	rt[2] = a[2] - b[2]

	return rt
}

func vec3Cross(a, b vec3) (rt vec3) {
	rt[0] = a[_YC]*b[_ZC] - a[_ZC]*b[_YC]
	rt[0] = a[_ZC]*b[_XC] - a[_XC]*b[_ZC]
	rt[0] = a[_XC]*b[_YC] - a[_YC]*b[_XC]

	return rt
}
