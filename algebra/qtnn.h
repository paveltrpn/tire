
#pragma once

#include <cmath>

#include "algebra2.h"
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
			data[_XC] = q[_XC];
			data[_YC] = q[_YC];
			data[_ZC] = q[_ZC];
			data[_WC] = q[_WC];

			return (*this);
		};

		qtnn(): 
			data{0.0, 0.0, 0.0, 0.0} {};
		
		qtnn(const float x, const float y, const float z, const float w): 
			data{x, y, z, w} {};

		qtnn(const vec3 &v):
			data{v[_XC], v[_YC], v[_ZC], 0.0} {};
		
		qtnn(const qtnn &q): 
			data{q[_XC], q[_YC], q[_ZC], q[_WC]} {};

		qtnn(const vec3 &ax, float phi) {
    		float sinhalfphi;

			sinhalfphi = sinf(phi * 0.5f);

			data[_WC] = cosf(phi * 0.5f);
			data[_XC] = ax[_XC] * sinhalfphi;
			data[_YC] = ax[_YC] * sinhalfphi;
			data[_ZC] = ax[_ZC] * sinhalfphi;

		};		

		qtnn(float yaw, float pitch, float roll);
		
		~qtnn () {};
	
		float lenght() {
			return sqrtf(data[_XC]*data[_XC] +
				 		 data[_YC]*data[_YC] +
				 		 data[_ZC]*data[_ZC] +
				 		 data[_WC]*data[_WC]);
		};

		void normalize() {
			qtnn rt;
			float len;

			len = lenght();

			if (len > f_eps) {
				rt[_WC] = data[_WC] / len;
				rt[_XC] = data[_XC] / len;
				rt[_YC] = data[_YC] / len;
				rt[_ZC] = data[_ZC] / len;
			} else {
				printf("qtnn::normalize(): quaternion is too short!");
			}
		};

		void invert() {
			data[_WC] =  data[_WC];
			data[_XC] = -data[_XC];
			data[_YC] = -data[_YC];
			data[_ZC] = -data[_ZC];
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
		float data[4];
};

qtnn	qtnnScale(const qtnn &q, float scale);
qtnn	qtnnSum(const qtnn &a, const qtnn &b);
qtnn 	qtnnSub(const qtnn &a, const qtnn &b);
float   qtnnDot(const qtnn &a, const qtnn &b);
qtnn  	qtnnMult(const qtnn &a, const qtnn &b);
qtnn 	qtnnSlerp(qtnn from, qtnn to, float t);
