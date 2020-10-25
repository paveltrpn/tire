
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

		plane_t(const vec3 &v, float d); /*empty*/

		plane_t(const vec3 &v1, const vec3 &v2, const vec3 &v3); /*empty*/

	private:
		float data[4];
};

vec3 	plane_ray_insct(const vec3 &rs, const vec3 &re); /*empty*/
bool	point_in_trngl(const vec3 &v1, 
					   const vec3 &v2, 
					   const vec3 &v3,
					   const vec3 &point);
