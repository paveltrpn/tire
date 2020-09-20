
#pragma once

#include <cmath>

#include "algebra2_common.h"

using namespace std;

class vec3 {
	public:
		//vec3(const vec3 &&v) = delete;
		//vec3 &operator=(const vec3 &&v) = delete;

		float operator[](const int32_t id) const {
			return data[id];
		};

		float &operator[](const int32_t id) {
			return data[id];
		};

		vec3 &operator=(const vec3 &v) {
			data[_XC] = v[_XC];
			data[_YC] = v[_YC];
			data[_ZC] = v[_ZC];

			return (*this);
		};

		vec3(): 
			data{0.0, 0.0, 0.0} {};
		
		vec3(const float x, const float y, const float z): 
			data{x, y, z} {};
		
		vec3(const vec3 &v): 
			data{v[_XC], v[_YC], v[_ZC]} {};

		~vec3() {};
	
	private:
		float data[3];
};

void   vec3_show(vec3 &v);
vec3 vec3_copy(vec3 &v);
vec3 vec3_set(float x, float y, float z);
float  vec3_lenght(vec3 &v);
vec3 vec3_normalize(vec3 &v);
vec3 vec3_scale(vec3 &v, float scale);
vec3 vec3_invert(vec3 &v);
float  vec3_dot(vec3 &a, vec3 &b);
vec3 vec3_sum(vec3 &a, vec3 &b);
vec3 vec3_sub(vec3 &a, vec3 &b);
vec3 vec3_cross(vec3 &a, vec3 &b);
