
#include <stdio.h>
#include <math.h>
#include "algebra_c.h"

void vec4Copy(const vec4 v, vec4 rt) {
	rt[0] = v[0];
	rt[1] = v[1];
	rt[2] = v[2];
	rt[3] = v[3];
}

void vec4Zero(vec4 v) {
	v[0] = 0.0f;
	v[1] = 0.0f;
	v[2] = 0.0f;
	v[3] = 0.0f;
}

void vec4Set(float x, float y, float z, float w, vec4 rt) {
	rt[0] = x;
	rt[1] = y;
	rt[2] = z;
	rt[3] = w;
}

float vec4Lenght(const vec4 v) {
	return sqrtf(v[0]*v[0] +
				 v[1]*v[1] +
				 v[2]*v[2] +
				 v[3]*v[3]);

}

void vec4Normalize(vec4 v) {
	float len;
	
	len = vec4Lenght(v);

	if (len > f_eps) {
		v[2] = v[2] / len;
		v[0] = v[0] / len;
		v[1] = v[1] / len;
		v[3] = v[3] / len;
	} else {
		return;
	}
}

void vec4Scale(const vec4 v, const float scale, vec4 rt) {
	rt[0] *= scale;
	rt[1] *= scale;
	rt[2] *= scale;
	rt[3] *= scale;
}

void vec4Invert(vec4 v) {
	v[0] = -v[0];
	v[1] = -v[1];
	v[2] = -v[2];
	v[3] = -v[3];
}

float vec4Dot(const vec4 a, const vec4 b) {
	return a[0]*b[0] + a[1]*b[1] + a[2]*b[2] + a[3]*b[3];
}

void vec4Sum(const vec4 a, const vec4 b, vec4 rt) {
	rt[0] = a[0] + b[0];
	rt[1] = a[1] + b[1];
	rt[2] = a[2] + b[2];
	rt[3] = a[3] + b[3];
}

void vec4Sub(const vec4 a, const vec4 b, vec4 rt) {
	rt[0] = a[0] - b[0];
	rt[1] = a[1] - b[1];
	rt[2] = a[2] - b[2];
	rt[3] = a[3] - b[3];
}
