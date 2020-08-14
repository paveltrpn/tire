
#include <iostream>
#include <cmath>

#include "vec3.h"

using namespace std;

void vec3_show(vec3_t &v) {
	printf("%5.2f %5.2f %5.2f\n", v[0], v[1], v[2]);
}

vec3_t vec3_copy(vec3_t &v) {
    vec3_t rt;

	rt[0] = v[0];
	rt[1] = v[1];
	rt[2] = v[2];

	return rt;
}

vec3_t vec3_set(float x, float y, float z) {
    vec3_t rt;

	rt[0] = x;
	rt[1] = y;
	rt[2] = z;

	return rt;
}

float vec3_lenght(vec3_t &v) {
	return sqrtf(v[_XC]*v[_XC] +
				 v[_YC]*v[_YC] +
				 v[_ZC]*v[_ZC]);

}

vec3_t vec3_normalize(vec3_t &v) {
	vec3_t rt;
	float len;

	len = vec3_lenght(v);

	if (len != 0.0) {
		rt[_ZC] = v[_ZC] / len;
		rt[_XC] = v[_XC] / len;
		rt[_YC] = v[_YC] / len;
	}

	return rt;
}

vec3_t vec3_scale(vec3_t &v, float scale) {
    vec3_t rt;

	rt[0] = v[0] * scale;
	rt[1] = v[1] * scale;
	rt[2] = v[2] * scale;

	return rt;
}

vec3_t vec3_invert(vec3_t &v) {
    vec3_t rt;

	rt[_XC] = -v[_XC];
	rt[_YC] = -v[_YC];
	rt[_ZC] = -v[_ZC];

	return rt;
}

float vec3_dot(vec3_t &a, vec3_t &b) {
	return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
}

vec3_t vec3_sum(vec3_t &a, vec3_t &b) {
    vec3_t rt;

	rt[0] = a[0] + b[0];
	rt[1] = a[1] + b[1];
	rt[2] = a[2] + b[2];

	return rt;
}

vec3_t vec3_sub(vec3_t &a, vec3_t &b) {
    vec3_t rt;

	rt[0] = a[0] - b[0];
	rt[1] = a[1] - b[1];
	rt[2] = a[2] - b[2];

	return rt;
}

vec3_t vec3_cross(vec3_t &a, vec3_t &b) {
    vec3_t rt;

	rt[0] = a[_YC]*b[_ZC] - a[_ZC]*b[_YC];
	rt[1] = a[_ZC]*b[_XC] - a[_XC]*b[_ZC];
	rt[2] = a[_XC]*b[_YC] - a[_YC]*b[_XC];

	return rt;
}