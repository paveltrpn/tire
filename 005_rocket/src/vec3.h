
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
	
		float length() {
			return sqrtf(data[_XC]*data[_XC] +
				 		 data[_YC]*data[_YC] +
				 		 data[_ZC]*data[_ZC]);
		};

		void normalize() {
			float len = length();

			if (len > f_eps) {
				data[_ZC] = data[_ZC] / len;
				data[_XC] = data[_XC] / len;
				data[_YC] = data[_YC] / len;
			}
		}

	private:
		float data[3];
};

vec3  vec3Scale(const vec3 &v, float scale);
vec3  vec3Invert(const vec3 &v);
float vec3Dot(const vec3 &a, const vec3 &b);
vec3  vec3Sum(const vec3 &a, const vec3 &b);
vec3  vec3Sub(const vec3 &a, const vec3 &b);
vec3  vec3Cross(const vec3 &a, const vec3 &b);
