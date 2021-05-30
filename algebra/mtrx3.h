
#pragma once

#include <cmath>
#include <tuple>

#include "algebra2.h"
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

mtrx3 mtrx3Idtt();
mtrx3 mtrx3Set(float m[4]);
mtrx3 mtrx3SetFloat(float a00, float a01, float a02,
	                    float a10, float a11, float a12,
	                    float a20, float a21, float a22);
mtrx3 mtrx3SetYaw(float angl);
mtrx3 mtrx3SetPitch(float angl);
mtrx3 mtrx3SetRoll(float angl);
float mtrx3Det(mtrx3 m);
float mtrx3DetLU(mtrx3 m); 
mtrx3 mtrx3Mult(mtrx3 a, mtrx3 b);
vec3  mtrx3MultVec(mtrx3 m, vec3 v);
tuple<mtrx3, mtrx3> mtrx3LU(mtrx3 m);
tuple<mtrx3, vec3> mtrx3LDLT(mtrx3 m);
mtrx3 mtrx3Transpose(mtrx3 m);
mtrx3 mtrx3Invert(mtrx3 m);
vec3  mtrx3SolveGauss(mtrx3 m, vec3 v);
mtrx3 mtrx3InsertCmn(mtrx3 m, vec3 v, int cmn);
vec3  mtrx3SolveKramer(mtrx3 m, vec3 v);
