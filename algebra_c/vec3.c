
#include <stdio.h>
#include <math.h>
#include "algebra_c.h"

void vec3_copy(const vec3 v, vec3 rt) {
	rt[0] = v[0];
	rt[1] = v[1];
	rt[2] = v[2];
}

void vec3Zero(vec3 v) {
	v[0] = 0.0f;
	v[1] = 0.0f;
	v[2] = 0.0f;
}

void vec3Set(float x, float y, float z, vec3 rt) {
	rt[0] = x;
	rt[1] = y;
	rt[2] = z;
}

float vec3Lenght(const vec3 v) {
	return sqrtf(v[0]*v[0] +
				 v[1]*v[1] +
				 v[2]*v[2]);

}

void vec3_normalize(vec3 v) {
	float len;
	
	len = vec3Lenght(v);

	if (len > f_eps) {
		v[2] = v[2] / len;
		v[0] = v[0] / len;
		v[1] = v[1] / len;
	} else {
		return;
	}
}

void vec3Scale(const vec3 v, const float scale, vec3 rt) {
	rt[0] *= scale;
	rt[1] *= scale;
	rt[2] *= scale;
}

void vec3Invert(vec3 v) {
	v[0] = -v[0];
	v[1] = -v[1];
	v[2] = -v[2];
}

float vec3Dot(const vec3 a, const vec3 b) {
	return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
}

void vec3Sum(const vec3 a, const vec3 b, vec3 rt) {
	rt[0] = a[0] + b[0];
	rt[1] = a[1] + b[1];
	rt[2] = a[2] + b[2];
}

void vec3Sub(const vec3 a, const vec3 b, vec3 rt) {
	rt[0] = a[0] - b[0];
	rt[1] = a[1] - b[1];
	rt[2] = a[2] - b[2];
}

void vec3_cross(const vec3 a, const vec3 b, vec3 rt) {
	rt[0] = a[1]*b[2] - a[2]*b[1];
	rt[1] = a[2]*b[0] - a[0]*b[2];
	rt[2] = a[0]*b[1] - a[1]*b[0];
}
