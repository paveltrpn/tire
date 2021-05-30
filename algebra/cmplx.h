
#pragma once

#include <cmath>

#include "algebra2.h"

using namespace std;

class cmplx {
	public:
		float operator[](const int32_t id) const {
			return data[id];
		};

		float &operator[](const int32_t id) {
			return data[id];
		};

		cmplx &operator=(const cmplx &v) {
			data[_XC] = v[_XC];
			data[_YC] = v[_YC];

			return (*this);
		};

		cmplx(): 
			data{0.0, 0.0} {};
		
		cmplx(const float x, const float y): 
			data{x, y} {};
		
		cmplx(const cmplx &v): 
			data{v[_XC], v[_YC]} {};

		~cmplx() {};
	
	private:
		float data[2];
};

cmplx cmplxSum(cmplx a, cmplx b);
cmplx cmplxSub(cmplx a, cmplx b);
cmplx cmplxMult(cmplx a, cmplx b);
cmplx cmplxDiv(cmplx a, cmplx b); 
cmplx cmplxScale(cmplx a, float k);
