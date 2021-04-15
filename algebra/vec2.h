
#pragma once

#include <cmath>

#include "algebra2_common.h"

using namespace std;

class vec2 {
	public:	
		float operator[](const int32_t id) const {
			return data[id];
		};

		float &operator[](const int32_t id) {
			return data[id];
		};

		vec2 &operator=(const vec2 &v) {
			data[_XC] = v[_XC];
			data[_YC] = v[_YC];

			return (*this);
		};

		vec2(): 
			data{0.0, 0.0} {};
		
		vec2(const float x, const float y): 
			data{x, y} {};
		
		vec2(const vec2 &v): 
			data{v[_XC], v[_YC]} {};

		~vec2() {};
	
	private:
		float data[2];
};

void   vec2_show(const vec2 &v);
float  vec2_lenght(const vec2 &v);
vec2 vec2_normalize(const vec2 &v);
vec2 vec2_scale(const vec2 &v,const float scale);
vec2 vec2_invert(const vec2 &v);
float  vec2_dot(const vec2 &a, const vec2 &b);
vec2 vec2_sum(const vec2 &a, const vec2 &b);
vec2 vec2_sub(const vec2 &a, const vec2 &b);
