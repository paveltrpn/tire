
#pragma once

#include <cmath>

#include "algebra2_common.h"
#include "vec3.h"

using namespace std;

class plane_t {
	public:
		float operator[](const int32_t id) const {
			return data[id];
		};

		float &operator[](const int32_t id) {
			return data[id];
		};

		plane_t &operator=(const plane_t &p) {
			data[_XC] = p[_XC];
			data[_YC] = p[_YC];
			data[_ZC] = p[_ZC];
			data[_WC] = p[_WC];

			return (*this);
		};

		plane_t(): 
			data{0.0, 0.0, 0.0, 0.0} {};
		
		plane_t(const float x, const float y, const float z, const float w): 
			data{x, y, z, w} {};

		plane_t(const vec3_t &v, float d); /*empty*/

		plane_t(const vec3_t &v1, const vec3_t &v2, const vec3_t &v3); /*empty*/

	private:
		float data[4];
};

vec3_t 	plane_ray_insct(const vec3_t &rs, const vec3_t &re); /*empty*/
bool	point_in_trngl(const vec3_t &v1, 
					   const vec3_t &v2, 
					   const vec3_t &v3,
					   const vec3_t &point);
