
#pragma once

#include <cmath>
#include <tuple>

#include "algebra2_common.h"
#include "vec3.h"

using namespace std;

class mtrx3_t {
	public:
		float operator[](const int32_t id) const {
			return data[id];
		};

		float &operator[](const int32_t id) {
			return data[id];
		};

		mtrx3_t &operator=(const mtrx3_t &m) {
			for (int i = 0; i < 9; i++) {
				data[i] = m[i];
			}
			
			return (*this);
		};

		mtrx3_t(): 
			data{1.0f, 0.0f, 0.0f,
				 0.0f, 1.0f, 0.0f,
				 0.0f, 0.0f, 1.0f} {};

		mtrx3_t(float a00, float a01, float a02,
                float a10, float a11, float a12,
                float a20, float a21, float a22):
			data{a00, a01, a02,
                 a10, a11, a12,
                 a20, a21, a22} {};

		mtrx3_t(const mtrx3_t &m):
			data{m[0], m[1], m[2],
				 m[3], m[4], m[5],
				 m[6], m[7], m[8]} {};
		
		mtrx3_t(float yaw, float pitch, float roll);
		mtrx3_t(const vec3_t &ax, float phi);
		
		~mtrx3_t() {};
	
	private:
		float data[9];
};

mtrx3_t mtrx3_idtt();
mtrx3_t mtrx3_set(float m[4]);
mtrx3_t mtrx3_set_float(float a00, float a01, float a02,
	                    float a10, float a11, float a12,
	                    float a20, float a21, float a22);
mtrx3_t mtrx3_set_yaw(float angl);
mtrx3_t mtrx3_set_pitch(float angl);
mtrx3_t mtrx3_set_roll(float angl);
void  mtrx3_show(mtrx3_t m);
float mtrx3_det(mtrx3_t m);
float mtrx3_det_lu(mtrx3_t m); 
mtrx3_t mtrx3_mult(mtrx3_t a, mtrx3_t b);
vec3_t  mtrx3_mult_vec(mtrx3_t m, vec3_t v);
tuple<mtrx3_t, mtrx3_t> mtrx3_lu(mtrx3_t m);
tuple<mtrx3_t, vec3_t> mtrx3_ldlt(mtrx3_t m);
mtrx3_t mtrx3Transpose(mtrx3_t m);
mtrx3_t mtrx3_invert(mtrx3_t m);
vec3_t  mtrx3_solve_gauss(mtrx3_t m, vec3_t v);
mtrx3_t mtrx3_insert_cmn(mtrx3_t m, vec3_t v, int cmn);
vec3_t  mtrx3_solve_kramer(mtrx3_t m, vec3_t v);
