
#pragma once

#include <cmath>

#include "algebra2_common.h"

using namespace std;

class vec4_t {
	public:
		float operator[](const int32_t id) const {
			return data[id];
		};

		float &operator[](const int32_t id) {
			return data[id];
		};

		vec4_t &operator=(const vec4_t &v) {
			data[_XC] = v[_XC];
			data[_YC] = v[_YC];
			data[_ZC] = v[_ZC];
			data[_WC] = v[_WC];

			return (*this);
		};

		vec4_t(): 
			data{0.0, 0.0, 0.0, 0.0} {};
		
		vec4_t(const float x, const float y, const float z, const float w): 
			data{x, y, z, w} {};
		
		vec4_t(const vec4_t &v): 
			data{v[_XC], v[_YC], v[_ZC], v[_WC]} {};

		~vec4_t() {};
	
	private:
		float data[4];
};

void   vec4_show(vec4_t &v);
vec4_t vec4_copy(vec4_t &v);
vec4_t vec4_set(float x, float y, float z, float w);
float  vec4_lenght(vec4_t &v);
vec4_t vec4_normalize(vec4_t &v);
vec4_t vec4_scale(vec4_t &v, float scale);
vec4_t vec4_invert(vec4_t &v);
float  vec4_dot(vec4_t &a, vec4_t &b);
vec4_t vec4_sum(vec4_t a, vec4_t b); 
vec4_t vec4Sub(vec4_t &a, vec4_t &b);
