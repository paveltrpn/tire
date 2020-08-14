
#include <iostream>
#include <cmath>

#include "vec2.h"

using namespace std;

void vec2_show(const vec2_t &v) {
	printf("%5.2f %5.2f\n", v[0], v[1]);
}

float vec2_lenght(const vec2_t &v) {
	return sqrt(vec2_dot(v,v));
}

vec2_t vec2_normalize(const vec2_t &v) {
	vec2_t rt;
	float len;
	
	len = vec2_lenght(v);

	if (len != 0.0f) {
		rt[_XC] = v[_XC] / len;
		rt[_YC] = v[_YC] / len;
	}

	return rt;
}

vec2_t	vec2_scale(const vec2_t &v,const float scale) {
	return vec2_t(v[0] * scale, v[1] * scale);
}

vec2_t vec_invert(const vec2_t &v) {
	return vec2_t(-v[0], -v[1]);
}

float vec2_dot(const vec2_t &a, const vec2_t &b) {
	return a[0] * b[0] + a[1] * b[1];;
}

vec2_t vec_sum(const vec2_t &a, const vec2_t &b) {
	return vec2_t(a[0] + b[0], a[1] + b[1]);
}

vec2_t vec2_sub(const vec2_t &a, const vec2_t &b) {
	return vec2_t(a[0] - b[0], a[1] - b[1]);
}

