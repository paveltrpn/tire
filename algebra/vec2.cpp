
#include <iostream>
#include <cmath>

#include "vec2.h"

float vec2_lenght(const vec2 &v) {
	return sqrt(vec2Dot(v,v));
}

vec2 vec2_normalize(const vec2 &v) {
	vec2 rt;
	float len;
	
	len = vec2_lenght(v);

	if (len != 0.0f) {
		rt[_XC] = v[_XC] / len;
		rt[_YC] = v[_YC] / len;
	}

	return rt;
}

vec2 vec2Scale(const vec2 &v,const float scale) {
	return vec2(v[0] * scale, v[1] * scale);
}

vec2 vecInvert(const vec2 &v) {
	return vec2(-v[0], -v[1]);
}

float vec2Dot(const vec2 &a, const vec2 &b) {
	return a[0] * b[0] + a[1] * b[1];;
}

vec2 vecSum(const vec2 &a, const vec2 &b) {
	return vec2(a[0] + b[0], a[1] + b[1]);
}

vec2 vec2Sub(const vec2 &a, const vec2 &b) {
	return vec2(a[0] - b[0], a[1] - b[1]);
}

