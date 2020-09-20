
#pragma once

#include <cmath>
#include <tuple>

#include "algebra2_common.h"
#include "vec3.h"
#include "vec4.h"

class mtrx4 {
	public:
		float operator[](const int32_t id) const {
			return data[id];
		};

		float &operator[](const int32_t id) {
			return data[id];
		};

		mtrx4 &operator=(const mtrx4 &m) {
			for (int i = 0; i < 16; i++) {
				data[i] = m[i];
			}
			
			return (*this);
		};

		mtrx4(): 
			data{1.0f, 0.0f, 0.0f, 0.0f,
				 0.0f, 1.0f, 0.0f, 0.0f,
				 0.0f, 0.0f, 1.0f, 0.0f,
				 0.0f, 0.0f, 0.0f, 1.0f} {};

		mtrx4(float a00, float a01, float a02, float a03,
                float a10, float a11, float a12, float a13,
                float a20, float a21, float a22, float a23,
                float a30, float a31, float a32, float a33):
			data {a00, a01, a02, a03,
                  a10, a11, a12, a13,
                  a20, a21, a22, a23,
                  a30, a31, a32, a33} {};

		mtrx4(const mtrx4 &m):
			data{m[0],  m[1],  m[2],  m[3], 
				 m[4],  m[5],  m[6],  m[7], 
				 m[8],  m[9],  m[10], m[11],
				 m[12], m[13], m[14], m[15]} {};
		
		mtrx4(float yaw, float pitch, float roll);
		mtrx4(const vec3 &ax, float phi);

		~mtrx4() {};
	
		
	private:
		float data[16];
};

mtrx4 mtrx4_idtt();
mtrx4 mtrx4_set(float m[16]);
mtrx4 mtrx4_set_float(float a00, float a01, float a02, float a03,
	                    float a10, float a11, float a12, float a13,
	                    float a20, float a21, float a22, float a23,
	                    float a30, float a31, float a32, float a33);
void mtrx4_show(mtrx4 m);
float mtrx4_det_lu(mtrx4 m);
mtrx4 mtrx4_mult(mtrx4 a, mtrx4 b);
vec4 mtrx4_mult_vec(mtrx4 m, vec3 v);
tuple<mtrx4, mtrx4> mtrx4_lu(mtrx4 m);
tuple<mtrx4, vec4> mtrx4_ldlt(mtrx4 m);
mtrx4 mtrx3_transpose(mtrx4 m);
vec4 mtrx4_solve_gauss(mtrx4 m, vec4 v);
mtrx4 mtrx4_insert_cmn(mtrx4 m, vec4 v, int cmn);
vec4 mtrx4_solve_kramer(mtrx4 m, vec4 v); 
