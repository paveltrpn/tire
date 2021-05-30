
#pragma once

#include <cmath>

#include "algebra2.h"

using namespace std;

class vec4 {
	public:
		float operator[](const int32_t id) const {
			return data[id];
		};

		float &operator[](const int32_t id) {
			return data[id];
		};

		vec4 &operator=(const vec4 &v) {
			data[_XC] = v[_XC];
			data[_YC] = v[_YC];
			data[_ZC] = v[_ZC];
			data[_WC] = v[_WC];

			return (*this);
		};

		vec4(): 
			data{0.0, 0.0, 0.0, 0.0} {};
		
		vec4(const float x, const float y, const float z, const float w): 
			data{x, y, z, w} {};
		
		vec4(const vec4 &v): 
			data{v[_XC], v[_YC], v[_ZC], v[_WC]} {};

		~vec4() {};
	
	private:
		float data[4];
};

void vec4Show(vec4 &v);
vec4 vec4Copy(vec4 &v);
vec4 vec4Set(float x, float y, float z, float w);
float  vec4Lenght(vec4 &v);
vec4 vec4Normalize(vec4 &v);
vec4 vec4Scale(vec4 &v, float scale);
vec4 vec4Invert(vec4 &v);
float  vec4Dot(vec4 &a, vec4 &b);
vec4 vec4Sum(vec4 a, vec4 b); 
vec4 vec4Sub(vec4 &a, vec4 &b);
