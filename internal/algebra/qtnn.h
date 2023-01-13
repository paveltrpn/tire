
#pragma once

#include <iostream>
#include <cmath>
#include <array>

#include "common.h"
#include "vec3.h"

using namespace std;

class qtnn {
	public:
		float operator[](const int32_t id) const {
			return data[id];
		};

		float &operator[](const int32_t id) {
			return data[id];
		};

		qtnn &operator=(const qtnn &q) {
			data[0] = q[0];
			data[1] = q[1];
			data[2] = q[2];
			data[3] = q[3];

			return (*this);
		};

		qtnn(): 
			data{0.0, 0.0, 0.0, 0.0} {};
		
		qtnn(const float x, const float y, const float z, const float w): 
			data{x, y, z, w} {};

		qtnn(const vec3 &v):
			data{v[0], v[1], v[2], 0.0} {};
		
		qtnn(const qtnn &q): 
			data{q[0], q[1], q[2], q[3]} {};

		qtnn(const vec3 &ax, float phi) {
    		float sinhalfphi;

			sinhalfphi = sinf(phi * 0.5f);

			data[3] = cosf(phi * 0.5f);
			data[0] = ax[0] * sinhalfphi;
			data[1] = ax[1] * sinhalfphi;
			data[2] = ax[2] * sinhalfphi;

		};		

		qtnn(float yaw, float pitch, float roll);
		
		~qtnn () {};
	
		float lenght() {
			return sqrtf(data[0]*data[0] +
				 		 data[1]*data[1] +
				 		 data[2]*data[2] +
				 		 data[3]*data[3]);
		};

		void normalize() {
			qtnn rt;
			float len;

			len = lenght();

			if (len > f_eps) {
				rt[3] = data[3] / len;
				rt[0] = data[0] / len;
				rt[1] = data[1] / len;
				rt[2] = data[2] / len;
			} else {
				std::cout << "qtnn::normalize(): quaternion is too short!";
			}
		};

		void invert() {
			data[3] =  data[3];
			data[0] = -data[0];
			data[1] = -data[1];
			data[2] = -data[2];
		};
		
		void setRotX(float phi) {
			float halfPhi = phi/2.0;

			data[3] = cosf(halfPhi);
			data[0] = sinf(halfPhi);
			data[1] = 0.0;
			data[2] = 0.0;
		}

		void setRotY(float phi) {
			float halfPhi = phi/2.0;

			data[3] = cosf(halfPhi);
			data[0] = 0.0;
			data[1] = sinf(halfPhi);
			data[2] = 0.0;
		}

		void setRotZ(float phi) {
			float halfPhi = phi/2.0;

			data[3] = cosf(halfPhi);
			data[0] = 0.0;
			data[1] = 0.0;
			data[2] = sinf(halfPhi);
		}

	private:
		std::array<float, 4> data;
};

qtnn	qtnnScale(const qtnn &q, float scale);
qtnn	qtnnSum(const qtnn &a, const qtnn &b);
qtnn 	qtnnSub(const qtnn &a, const qtnn &b);
float   qtnnDot(const qtnn &a, const qtnn &b);
qtnn  	qtnnMult(const qtnn &a, const qtnn &b);
qtnn 	qtnnSlerp(qtnn from, qtnn to, float t);
