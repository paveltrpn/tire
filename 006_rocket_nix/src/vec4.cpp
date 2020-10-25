
#include <iostream>
#include <cmath>

#include "vec4.h"

using namespace std;

void vec4_show(vec4 &v) {
	printf("%5.2f %5.2f %5.2f %5.2f\n", v[_XC], v[_YC], v[_ZC], v[_WC]);
}

vec4 vec4_copy(vec4 &v) {
    vec4 rt;

	rt[0] = v[0];
	rt[1] = v[1];
	rt[2] = v[2];
	rt[3] = v[3];

	return rt;
}

vec4 vec4_set(float x, float y, float z, float w) {
    vec4 rt;

	rt[0] = x;
	rt[1] = y;
	rt[2] = z;
	rt[3] = w;

	return rt;
}

float vec4_lenght(vec4 &v) {
	return sqrtf(v[_XC]*v[_XC] +
		v[_YC]*v[_YC] +
		v[_ZC]*v[_ZC] +
		v[_WC]*v[_WC]);
}

vec4 vec4_normalize(vec4 &v) {
	vec4 rt;
	float len;

	len = vec4_lenght(v);

	if (len != 0.0f) {
		rt[_XC] = v[_XC] / len;
		rt[_YC] = v[_YC] / len;
		rt[_ZC] = v[_ZC] / len;
		rt[_WC] = v[_WC] / len;
	}

	return rt;
}

vec4 vec4_scale(vec4 &v, float scale) {
    vec4 rt;

	rt[0] = v[0] * scale;
	rt[1] = v[1] * scale;
	rt[2] = v[2] * scale;
	rt[3] = v[3] * scale;

	return rt;
}

vec4 vec4_invert(vec4 &v) {
    vec4 rt;

	rt[_XC] = -v[_XC];
	rt[_YC] = -v[_YC];
	rt[_ZC] = -v[_ZC];
	rt[_WC] = -v[_WC];

	return rt;
}

float vec4_dot(vec4 &a, vec4 &b) {
	return a[0]*b[0] + a[1]*b[1] + a[2]*b[2] + a[3]*b[3];
}

vec4 vec4_sum(vec4 a, vec4 b) {
    vec4 rt;

	rt[0] = a[0] + b[0];
	rt[1] = a[1] + b[1];
	rt[2] = a[2] + b[2];
	rt[3] = a[3] + b[3];

	return rt;
}

vec4 vec4Sub(vec4 &a, vec4 &b) {
    vec4 rt;

	rt[0] = a[0] - b[0];
	rt[1] = a[1] - b[1];
	rt[2] = a[2] - b[2];
	rt[3] = a[3] - b[3];

	return rt;
}
