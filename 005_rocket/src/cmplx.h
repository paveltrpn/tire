
#pragma once

#include <cmath>

#include "algebra2_common.h"

using namespace std;

class cmplx_t {
	public:
		float operator[](const int32_t id) const {
			return data[id];
		};

		float &operator[](const int32_t id) {
			return data[id];
		};

		cmplx_t &operator=(const cmplx_t &v) {
			data[_XC] = v[_XC];
			data[_YC] = v[_YC];

			return (*this);
		};

		cmplx_t(): 
			data{0.0, 0.0} {};
		
		cmplx_t(const float x, const float y): 
			data{x, y} {};
		
		cmplx_t(const cmplx_t &v): 
			data{v[_XC], v[_YC]} {};

		~cmplx_t() {};
	
	private:
		float data[2];
};

cmplx_t cmplx_sum(cmplx_t a, cmplx_t b);
cmplx_t cmplx_sub(cmplx_t a, cmplx_t b);
cmplx_t cmplx_mult(cmplx_t a, cmplx_t b);
cmplx_t cmplx_div(cmplx_t a, cmplx_t b); 
cmplx_t cmplx_scale(cmplx_t a, float k);
