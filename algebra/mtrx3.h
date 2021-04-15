
#pragma once

#include <cmath>
#include <tuple>

#include "algebra2_common.h"
#include "vec3.h"

using namespace std;

class mtrx3 {
	public:
		float operator[](const int32_t id) const {
			return data[id];
		};

		float &operator[](const int32_t id) {
			return data[id];
		};

		mtrx3 &operator=(const mtrx3 &m) {
			for (int i = 0; i < 9; i++) {
				data[i] = m[i];
			}
			
			return (*this);
		};

		mtrx3(): 
			data{1.0f, 0.0f, 0.0f,
				 0.0f, 1.0f, 0.0f,
				 0.0f, 0.0f, 1.0f} {};

		mtrx3(float a00, float a01, float a02,
                float a10, float a11, float a12,
                float a20, float a21, float a22):
			data{a00, a01, a02,
                 a10, a11, a12,
                 a20, a21, a22} {};

		mtrx3(const mtrx3 &m):
			data{m[0], m[1], m[2],
				 m[3], m[4], m[5],
				 m[6], m[7], m[8]} {};
		
		mtrx3(float yaw, float pitch, float roll);
		mtrx3(const vec3 &ax, float phi);
		
		~mtrx3() {};
	
	private:
		float data[9];
};

mtrx3 mtrx3_idtt();
mtrx3 mtrx3_set(float m[4]);
mtrx3 mtrx3_set_float(float a00, float a01, float a02,
	                    float a10, float a11, float a12,
	                    float a20, float a21, float a22);
mtrx3 mtrx3_set_yaw(float angl);
mtrx3 mtrx3_set_pitch(float angl);
mtrx3 mtrx3_set_roll(float angl);
void  mtrx3_show(mtrx3 m);
float mtrx3_det(mtrx3 m);
float mtrx3_det_lu(mtrx3 m); 
mtrx3 mtrx3_mult(mtrx3 a, mtrx3 b);
vec3  mtrx3_mult_vec(mtrx3 m, vec3 v);
tuple<mtrx3, mtrx3> mtrx3_lu(mtrx3 m);
tuple<mtrx3, vec3> mtrx3_ldlt(mtrx3 m);
mtrx3 mtrx3Transpose(mtrx3 m);
mtrx3 mtrx3_invert(mtrx3 m);
vec3  mtrx3_solve_gauss(mtrx3 m, vec3 v);
mtrx3 mtrx3_insert_cmn(mtrx3 m, vec3 v, int cmn);
vec3  mtrx3_solve_kramer(mtrx3 m, vec3 v);
