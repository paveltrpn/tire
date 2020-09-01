
#pragma once

#include <cmath>
#include <tuple>

#include "algebra2_common.h"
#include "vec2.h"
#include "vec3.h"

using namespace std;

class mtrx2_t {
	public:
		float operator[](const int32_t id) const {
			return data[id];
		};

		float &operator[](const int32_t id) {
			return data[id];
		};

		mtrx2_t &operator=(const mtrx2_t &m) {
			for (int i = 0; i < 4; i++) {
				data[i] = m[i];
			}
			
			return (*this);
		};

		mtrx2_t(): 
			data{1.0f, 0.0f,
				 0.0f, 1.0f} {};

		mtrx2_t(float a00, float a01,
                float a10, float a11):
			data{a00, a01,
                 a10, a11} {};

		mtrx2_t(const mtrx2_t &m):
			data{m[0], m[1],
				 m[2], m[3]} {};
		
		mtrx2_t(float phi);
		
		~mtrx2_t() {};
	
	private:
		float data[4];
};

mtrx2_t mtrx2_idtt();
mtrx2_t mtrx2_set(float m[4]);
mtrx2_t mtrx2_set_float(float a00, float a01, float a10, float a11);
mtrx2_t mtrx2_rtn(float phi);
void 	mtrx2_show(mtrx2_t m);
float 	mtrx2_det(mtrx2_t m); 
float   mtrx2_det_lu(mtrx2_t m);
mtrx2_t mtrx2_mult(mtrx2_t a, mtrx2_t b);
vec2_t  mtrx2_mult_vec(mtrx2_t m, vec2_t v);
tuple<mtrx2_t, mtrx2_t> mtrx2_lu(mtrx2_t m);
tuple<mtrx2_t, vec2_t> mtrx2_ldlt(mtrx2_t m);
mtrx2_t mtrx2Transpose(mtrx2_t m);
mtrx2_t mtrx2Invert(mtrx2_t m);
vec2_t  mtrx2_solve_gauss(mtrx2_t m, vec2_t v);
mtrx2_t mtrx2_insert_cmn(mtrx2_t m, vec3_t v, int cmn);
vec2_t  mtrx2_solve_kramer(mtrx2_t m, vec3_t v);
