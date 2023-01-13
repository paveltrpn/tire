
#include <stdio.h>
#include <math.h>
#include "algebra_c.h"

void vec2Copy(const vec2 v, vec2 rt) {
	rt[0] = v[0];
	rt[1] = v[1];
}

void vec2Zero(vec2 v) {
	v[0] = 0.0f;
	v[1] = 0.0f;
}

void vec2Set(float x, float y, vec3 rt) {
	rt[0] = x;
	rt[1] = y;
}

float vec2Lenght(const vec2 v) {
	return sqrtf(v[0]*v[0] +
				 v[1]*v[1]);

}

void vec2Normalize(vec2 v) {
	float len;
	
	len = vec2Lenght(v);

	if (len > f_eps) {
		v[2] = v[2] / len;
		v[0] = v[0] / len;
	} else {
		return;
	}
}

void vec2Scale(const vec2 v, const float scale, vec2 rt) {
	rt[0] *= scale;
	rt[1] *= scale;
}

void vec2Invert(vec2 v) {
	v[0] = -v[0];
	v[1] = -v[1];
}

float vec2Dot(const vec2 a, const vec2 b) {
	return a[0]*b[0] + a[1]*b[1];
}

void vec2Sum(const vec2 a, const vec2 b, vec2 rt) {
	rt[0] = a[0] + b[0];
	rt[1] = a[1] + b[1];
}

void vec2Sub(const vec2 a, const vec2 b, vec2 rt) {
	rt[0] = a[0] - b[0];
	rt[1] = a[1] - b[1];
}

