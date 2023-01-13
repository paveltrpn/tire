
#pragma once

#include <cmath>
#include <array>

#include "common.h"
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
			data[0] = p[0];
			data[1] = p[1];
			data[2] = p[2];
			data[3] = p[3];

			return (*this);
		};

		plane_t(): 
			data{0.0, 0.0, 0.0, 0.0} {};
		
		plane_t(const float x, const float y, const float z, const float w): 
			data{x, y, z, w} {};

		plane_t(const vec3 &v, float d); /*empty*/

		plane_t(const vec3 &v1, const vec3 &v2, const vec3 &v3); /*empty*/

	private:
		std::array<float, 4> data;
};

vec3 	planeRayInsct(const vec3 &rs, const vec3 &re); /*empty*/
bool	pointInTrngl(const vec3 &v1, 
					   const vec3 &v2, 
					   const vec3 &v3,
					   const vec3 &point);
