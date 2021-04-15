
#pragma once

#include <cmath>
#include <tuple>

#include "algebra2_common.h"
#include "vec2.h"
#include "vec3.h"

using namespace std;

class mtrx2 {
	public:
		float operator[](const int32_t id) const {
			return data[id];
		};

		float &operator[](const int32_t id) {
			return data[id];
		};

		mtrx2 &operator=(const mtrx2 &m) {
			for (int i = 0; i < 4; i++) {
				data[i] = m[i];
			}
			
			return (*this);
		};

		mtrx2(): 
			data{1.0f, 0.0f,
				 0.0f, 1.0f} {};

		mtrx2(float a00, float a01,
                float a10, float a11):
			data{a00, a01,
                 a10, a11} {};

		mtrx2(const mtrx2 &m):
			data{m[0], m[1],
				 m[2], m[3]} {};
		
		mtrx2(float phi);
		
		~mtrx2() {};
	
	private:
		float data[4];
};

mtrx2 mtrx2_idtt();
mtrx2 mtrx2_set(float m[4]);
mtrx2 mtrx2_set_float(float a00, float a01, float a10, float a11);
mtrx2 mtrx2_rtn(float phi);
void 	mtrx2_show(mtrx2 m);
float 	mtrx2_det(mtrx2 m); 
float   mtrx2_det_lu(mtrx2 m);
mtrx2 mtrx2_mult(mtrx2 a, mtrx2 b);
vec2  mtrx2_mult_vec(mtrx2 m, vec2 v);
tuple<mtrx2, mtrx2> mtrx2_lu(mtrx2 m);
tuple<mtrx2, vec2> mtrx2_ldlt(mtrx2 m);
mtrx2 mtrx2Transpose(mtrx2 m);
mtrx2 mtrx2Invert(mtrx2 m);
vec2  mtrx2_solve_gauss(mtrx2 m, vec2 v);
mtrx2 mtrx2_insert_cmn(mtrx2 m, vec3 v, int cmn);
vec2  mtrx2_solve_kramer(mtrx2 m, vec3 v);
