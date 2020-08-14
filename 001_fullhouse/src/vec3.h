
#pragma once

#include <cmath>

#include "algebra2_common.h"

using namespace std;

class vec3_t {
	public:
		float operator[](const int32_t id) const {
			return data[id];
		};

		float &operator[](const int32_t id) {
			return data[id];
		};

		vec3_t &operator=(const vec3_t &v) {
			data[_XC] = v[_XC];
			data[_YC] = v[_YC];
			data[_ZC] = v[_ZC];

			return (*this);
		};

		vec3_t(): 
			data{0.0, 0.0, 0.0} {};
		
		vec3_t(const float x, const float y, const float z): 
			data{x, y, z} {};
		
		vec3_t(const vec3_t &v): 
			data{v[_XC], v[_YC], v[_ZC]} {};

		~vec3_t() {};
	
	private:
		float data[3];
};

void   vec3_show(vec3_t &v);
vec3_t vec3_copy(vec3_t &v);
vec3_t vec3_set(float x, float y, float z);
float  vec3_lenght(vec3_t &v);
vec3_t vec3_normalize(vec3_t &v);
vec3_t vec3_scale(vec3_t &v, float scale);
vec3_t vec3_invert(vec3_t &v);
float  vec3_dot(vec3_t &a, vec3_t &b);
vec3_t vec3_sum(vec3_t &a, vec3_t &b);
vec3_t vec3_sub(vec3_t &a, vec3_t &b);
vec3_t vec3_cross(vec3_t &a, vec3_t &b);
