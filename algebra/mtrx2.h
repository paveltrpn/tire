
#pragma once

#include <cmath>
#include <tuple>
#include <array>

#include "common.h"
#include "vec2.h"
#include "vec3.h"

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
		
		mtrx2(float arg) {
			for (auto &elem: data){
				elem = arg;
			}
		};
		
		~mtrx2() {};
	
	private:
		std::array<float, 4> data;
};

mtrx2 mtrx2FromIdtt();

mtrx2 mtrx2Rtn(float phi);
float 	mtrx2Det(mtrx2 m); 
float   mtrx2DetLU(mtrx2 m);
mtrx2 mtrx2Mult(mtrx2 a, mtrx2 b);
vec2  mtrx2MultVec(mtrx2 m, vec2 v);
std::tuple<mtrx2, mtrx2> mtrx2LU(mtrx2 m);
std::tuple<mtrx2, vec2> mtrx2LDLT(mtrx2 m);
mtrx2 mtrx2Transpose(mtrx2 m);
mtrx2 mtrx2Invert(mtrx2 m);
vec2  mtrx2SolveGauss(mtrx2 m, vec2 v);
mtrx2 mtrx2InsertCmn(mtrx2 m, vec3 v, int cmn);
vec2  mtrx2SolveKramer(mtrx2 m, vec3 v);
