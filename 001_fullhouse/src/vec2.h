
#pragma once

#include <cmath>

#include "algebra2_common.h"

using namespace std;

class vec2_t {
	public:
		float operator[](const int32_t id) const {
			return data[id];
		};

		float &operator[](const int32_t id) {
			return data[id];
		};

		vec2_t &operator=(const vec2_t &v) {
			data[_XC] = v[_XC];
			data[_YC] = v[_YC];

			return (*this);
		};

		vec2_t(): 
			data{0.0, 0.0} {};
		
		vec2_t(const float x, const float y): 
			data{x, y} {};
		
		vec2_t(const vec2_t &v): 
			data{v[_XC], v[_YC]} {};

		~vec2_t() {};
	
	private:
		float data[2];
};

void   vec2_show(const vec2_t &v);
float  vec2_lenght(const vec2_t &v);
vec2_t vec2_normalize(const vec2_t &v);
vec2_t vec2_scale(const vec2_t &v,const float scale);
vec2_t vec2_invert(const vec2_t &v);
float  vec2_dot(const vec2_t &a, const vec2_t &b);
vec2_t vec2_sum(const vec2_t &a, const vec2_t &b);
vec2_t vec2_sub(const vec2_t &a, const vec2_t &b);
