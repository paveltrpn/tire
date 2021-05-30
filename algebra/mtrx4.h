
#pragma once

#include <cmath>
#include <tuple>

#include "algebra2.h"
#include "vec3.h"
#include "vec4.h"
#include "qtnn.h"

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
		
		mtrx4(float yaw, float pitch, float roll) {
			float cosy, siny, cosp, sinp, cosr, sinr;

			cosy = cosf(degToRad(yaw));
			siny = sinf(degToRad(yaw));
			cosp = cosf(degToRad(pitch));
			sinp = sinf(degToRad(pitch));
			cosr = cosf(degToRad(roll));
			sinr = sinf(degToRad(roll));

			data[0]  = cosy*cosr - siny*cosp*sinr;
			data[1]  = -cosy*sinr - siny*cosp*cosr;
			data[2]  = siny * sinp;
			data[3]  = 0.0f;

			data[4]  = siny*cosr + cosy*cosp*sinr;
			data[5]  = -siny*sinr + cosy*cosp*cosr;
			data[6]  = -cosy * sinp;
			data[7]  = 0.0f;

			data[8]  = sinp * sinr;
			data[9]  = sinp * cosr;
			data[10] = cosp;
			data[11] = 0.0f;

			data[12] = 0.0f;
			data[13] = 0.0f;
			data[14] = 0.0f;
			data[15] = 1.0f;
		};

		mtrx4(const vec3 &ax, float phi) {
			float cosphi, sinphi, vxvy, vxvz, vyvz, vx, vy, vz;

			cosphi = cosf(degToRad(phi));
			sinphi = sinf(degToRad(phi));
			vxvy = ax[_XC] * ax[_YC];
			vxvz = ax[_XC] * ax[_ZC];
			vyvz = ax[_YC] * ax[_ZC];
			vx = ax[_XC];
			vy = ax[_YC];
			vz = ax[_ZC];

			data[0]  = cosphi + (1.0-cosphi)*vx*vx;
			data[1]  = (1.0-cosphi)*vxvy - sinphi*vz;
			data[2]  = (1.0-cosphi)*vxvz + sinphi*vy;
			data[3]  = 0.0f;

			data[4]  = (1.0-cosphi)*vxvy + sinphi*vz;
			data[5]  = cosphi + (1.0-cosphi)*vy*vy;
			data[6]  = (1.0-cosphi)*vyvz - sinphi*vx;
			data[7]  = 0.0f;

			data[8]  = (1.0-cosphi)*vxvz - sinphi*vy;
			data[9]  = (1.0-cosphi)*vyvz + sinphi*vx;
			data[10] = cosphi + (1.0-cosphi)*vz*vz;
			data[11] = 0.0f;

			data[12] = 0.0f;
			data[13] = 0.0f;
			data[14] = 0.0f;
			data[15] = 1.0f;
		};

		~mtrx4() {};
	
		void setIdtt() {
        	size_t i, j;

        	for (i = 0; i < order; i++) {
	    		for (j = 0; j < order; j++) {
	    			if (i == j) {
	    				data[idRw(i, j, order)] = 1.0f;
	    			} else {
	    				data[idRw(i, j, order)] = 0.0f;
	    			}
	    		}
        	}
    	};

		void fromQtnn(const qtnn &src) {
			float wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;

        	x2 = src[0] + src[0];
        	y2 = src[1] + src[1];
        	z2 = src[2] + src[2];
        	xx = src[0] * x2;   xy = src[0] * y2;   xz = src[0] * z2;
        	yy = src[1] * y2;   yz = src[1] * z2;   zz = src[2] * z2;
        	wx = src[3] * x2;   wy = src[3] * y2;   wz = src[3] * z2;

        	data[0]=1.0-(yy+zz);  data[1]=xy-wz;        data[ 2]=xz+wy;       data[ 3] = 0.0;
        	data[4]=xy+wz;        data[5]=1.0-(xx+zz);  data[ 6]=yz-wx;       data[ 7] = 0.0;
        	data[8]=xz-wy;        data[9]=yz+wx;        data[10]=1.0-(xx+yy); data[11] = 0.0;

        	data[12] = 0.0; data[13] = 0.0; data[14] = 0.0; data[15] = 1.0;
		};

		void setPerspective(float fovy, float aspect, float near, float far) {
        	float f = 1.0 / tanf(fovy / 2.0f);
        	float nf;
	
        	data[0] = f / aspect;
        	data[1] = 0;
        	data[2] = 0;
        	data[3] = 0;
        	data[4] = 0;
        	data[5] = f;
        	data[6] = 0;
        	data[7] = 0;
        	data[8] = 0;
        	data[9] = 0;
        	data[11] = -1;
        	data[12] = 0;
        	data[13] = 0;
        	data[15] = 0;
	
        	// if (far != 0.0f) {
        	    // nf = 1.0f / (near - far);
        	    // data[10] = (far + near) * nf;
        	    // data[14] = 2.0f * far * near * nf;
        	// } else {
        	    // data[10] = -1.0f;
        	    // data[14] = -2.0f * near;
        	// }
    	};

		void setTranslate(vec3 src) {
        	setIdtt();

        	data[3] = src[0];
        	data[7] = src[1];
        	data[11] = src[2];
    	}

		void invert() {
        	mtrx4 inv;
        	size_t i;
			float det;
	
        	inv[0] = data[5]  * data[10] * data[15] - 
        	         data[5]  * data[11] * data[14] - 
        	         data[9]  * data[6]  * data[15] + 
        	         data[9]  * data[7]  * data[14] +
        	         data[13] * data[6]  * data[11] - 
        	         data[13] * data[7]  * data[10];
	
        	inv[4] = -data[4]  * data[10] * data[15] + 
        	          data[4]  * data[11] * data[14] + 
        	          data[8]  * data[6]  * data[15] - 
        	          data[8]  * data[7]  * data[14] - 
        	          data[12] * data[6]  * data[11] + 
        	          data[12] * data[7]  * data[10];
	
        	inv[8] = data[4]  * data[9] * data[15] - 
        	         data[4]  * data[11] * data[13] - 
        	         data[8]  * data[5] * data[15] + 
        	         data[8]  * data[7] * data[13] + 
        	         data[12] * data[5] * data[11] - 
        	         data[12] * data[7] * data[9];
	
        	inv[12] = -data[4]  * data[9] * data[14] + 
        	           data[4]  * data[10] * data[13] +
        	           data[8]  * data[5] * data[14] - 
        	           data[8]  * data[6] * data[13] - 
        	           data[12] * data[5] * data[10] + 
        	           data[12] * data[6] * data[9];
	
        	inv[1] = -data[1]  * data[10] * data[15] + 
        	          data[1]  * data[11] * data[14] + 
        	          data[9]  * data[2] * data[15] - 
        	          data[9]  * data[3] * data[14] - 
        	          data[13] * data[2] * data[11] + 
        	          data[13] * data[3] * data[10];
	
        	inv[5] = data[0]  * data[10] * data[15] - 
        	         data[0]  * data[11] * data[14] - 
        	         data[8]  * data[2] * data[15] + 
        	         data[8]  * data[3] * data[14] + 
        	         data[12] * data[2] * data[11] - 
        	         data[12] * data[3] * data[10];
	
        	inv[9] = -data[0]  * data[9] * data[15] + 
        	          data[0]  * data[11] * data[13] + 
        	          data[8]  * data[1] * data[15] - 
        	          data[8]  * data[3] * data[13] - 
        	          data[12] * data[1] * data[11] + 
        	          data[12] * data[3] * data[9];
	
        	inv[13] = data[0]  * data[9] * data[14] - 
        	          data[0]  * data[10] * data[13] - 
        	          data[8]  * data[1] * data[14] + 
        	          data[8]  * data[2] * data[13] + 
        	          data[12] * data[1] * data[10] - 
        	          data[12] * data[2] * data[9];
	
        	inv[2] = data[1]  * data[6] * data[15] - 
        	         data[1]  * data[7] * data[14] - 
        	         data[5]  * data[2] * data[15] + 
        	         data[5]  * data[3] * data[14] + 
        	         data[13] * data[2] * data[7] - 
        	         data[13] * data[3] * data[6];
	
        	inv[6] = -data[0]  * data[6] * data[15] + 
        	          data[0]  * data[7] * data[14] + 
        	          data[4]  * data[2] * data[15] - 
        	          data[4]  * data[3] * data[14] - 
        	          data[12] * data[2] * data[7] + 
        	          data[12] * data[3] * data[6];
	
        	inv[10] = data[0]  * data[5] * data[15] - 
        	          data[0]  * data[7] * data[13] - 
        	          data[4]  * data[1] * data[15] + 
        	          data[4]  * data[3] * data[13] + 
        	          data[12] * data[1] * data[7] - 
        	          data[12] * data[3] * data[5];
	
        	inv[14] = -data[0]  * data[5] * data[14] + 
        	           data[0]  * data[6] * data[13] + 
        	           data[4]  * data[1] * data[14] - 
        	           data[4]  * data[2] * data[13] - 
        	           data[12] * data[1] * data[6] + 
        	           data[12] * data[2] * data[5];
	
        	inv[3] = -data[1] * data[6] * data[11] + 
        	          data[1] * data[7] * data[10] + 
        	          data[5] * data[2] * data[11] - 
        	          data[5] * data[3] * data[10] - 
        	          data[9] * data[2] * data[7] + 
        	          data[9] * data[3] * data[6];
	
        	inv[7] = data[0] * data[6] * data[11] - 
        	         data[0] * data[7] * data[10] - 
        	         data[4] * data[2] * data[11] + 
        	         data[4] * data[3] * data[10] + 
        	         data[8] * data[2] * data[7] - 
        	         data[8] * data[3] * data[6];
	
        	inv[11] = -data[0] * data[5] * data[11] + 
        	           data[0] * data[7] * data[9] + 
        	           data[4] * data[1] * data[11] - 
        	           data[4] * data[3] * data[9] - 
        	           data[8] * data[1] * data[7] + 
        	           data[8] * data[3] * data[5];
	
        	inv[15] = data[0] * data[5] * data[10] - 
        	          data[0] * data[6] * data[9] - 
        	          data[4] * data[1] * data[10] + 
        	          data[4] * data[2] * data[9] + 
        	          data[8] * data[1] * data[6] - 
        	          data[8] * data[2] * data[5];
	
        	det = data[0] * inv[0] + data[1] * inv[4] + data[2] * inv[8] + data[3] * inv[12];
	
        	if (det == 0) {
        	    setIdtt();
        	    return;
        	}
	
        	det = 1.0 / det;
	
        	for (i = 0; i < 16; i++)
        	    data[i] = inv[i] * det;
    	};	

		void transpose() {
        	mtrx4 tmp;

        	tmp[ 0] = data[ 0];
        	tmp[ 1] = data[ 4];
        	tmp[ 2] = data[ 8];
        	tmp[ 3] = data[12];
        	tmp[ 4] = data[ 1];
        	tmp[ 5] = data[ 5];
        	tmp[ 6] = data[ 9];
        	tmp[ 7] = data[13];
        	tmp[ 8] = data[ 2];
        	tmp[ 9] = data[ 6];
        	tmp[10] = data[10];
        	tmp[11] = data[14];
        	tmp[12] = data[ 3];
        	tmp[13] = data[ 7];
        	tmp[14] = data[11];
        	tmp[15] = data[15];

        	// for (size_t i = 0; i < 16; i++) {
        	    // data[i] = tmp[i];
        	// }

			(*this) = tmp;
    	};

	private:
		float data[16];
		const size_t order = 4;
};

float mtrx4DetLU(mtrx4 m);
mtrx4 mtrx4Mult(mtrx4 a, mtrx4 b);
vec4 mtrx4MultVec(mtrx4 m, vec3 v);
vec3 mtrx4MultVec3(mtrx4 m, vec3 v);
tuple<mtrx4, mtrx4> mtrx4LU(mtrx4 m);
tuple<mtrx4, vec4> mtrx4LDLT(mtrx4 m);
mtrx4 mtrx4Transpose(mtrx4 m);
vec4 mtrx4SolveGauss(mtrx4 m, vec4 v);
mtrx4 mtrx4InsertCmn(mtrx4 m, vec4 v, int cmn);
vec4 mtrx4SolveKramer(mtrx4 m, vec4 v); 
