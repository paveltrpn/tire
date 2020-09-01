
#pragma once

#include <cmath>

#include "algebra2_common.h"
#include "vec2.h"
#include "vec3.h"

using namespace std;

class qtnn_t {
	public:
		float operator[](const int32_t id) const {
			return data[id];
		};

		float &operator[](const int32_t id) {
			return data[id];
		};

		qtnn_t &operator=(const qtnn_t &q) {
			data[_XC] = q[_XC];
			data[_YC] = q[_YC];
			data[_ZC] = q[_ZC];
			data[_WC] = q[_WC];

			return (*this);
		};

		qtnn_t(): 
			data{0.0, 0.0, 0.0, 0.0} {};
		
		qtnn_t(const float x, const float y, const float z, const float w): 
			data{x, y, z, w} {};

		qtnn_t(const vec3_t &v):
			data{v[_XC], v[_YC], v[_ZC], 0.0} {};
		
		qtnn_t(const qtnn_t &q): 
			data{q[_XC], q[_YC], q[_ZC], q[_WC]} {};

		qtnn_t(float yaw, float pitch, float roll);
		qtnn_t(const vec3_t &ax, float phi);
		
		~qtnn_t () {};
	
	private:
		float data[4];
};

void 	qtnn_show(const qtnn_t &q); 
float	qtnn_lenght(const qtnn_t &q);
qtnn_t	qtnn_normalize(const qtnn_t &q);
qtnn_t	qtnn_invert(const qtnn_t &q);
qtnn_t	qtnn_scale(const qtnn_t &q, float scale);
qtnn_t	qtnn_sum(const qtnn_t &a, const qtnn_t &b);
qtnn_t 	qtnn_sub(const qtnn_t &a, const qtnn_t &b);
float   qtnn_dot(const qtnn_t &a, const qtnn_t &b);
qtnn_t  qtnn_mult(const qtnn_t &a, const qtnn_t &b); 
qtnn_t  qtnn_mult_vec3(const qtnn_t a, const qtnn_t &b);
vec3_t  qtnn_to_vec3(const qtnn_t &q);
vec3_t  qtnn_transform_vec3(const qtnn_t &a, const vec3_t &b);
