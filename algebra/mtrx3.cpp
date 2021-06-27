
#include <iostream>
#include <tuple>
#include <cmath>

#include "mtrx3.h"
#include "vec2.h"

using namespace std;

constexpr size_t mrange = 3;

mtrx3 mtrx3::operator*(mtrx3 &arg) {
   	mtrx3 rt = {0.0f};

	for (size_t i = 0; i < mrange; i++) {
		for (size_t  j = 0; j < mrange; j++) {
			for (size_t  k = 0; k < mrange; k++) {
				rt[idRw(i, j, mrange)] += (*this)[idRw(k, j, mrange)]*arg[idRw(i, k, mrange)];
			}
		}
	}

	return rt;
};

mtrx3 mtrx3FromIdtt() {
	mtrx3 rt;

	for (size_t i = 0; i < mrange; i++) {
		for (size_t j = 0; j < mrange; j++) {
			if (i == j) {
				rt[idRw(i, j, mrange)] = 1.0f;
			} else {
				rt[idRw(i, j, mrange)] = 0.0f;
			}
		}
	}

	return rt;
}

mtrx3 mtrx3FromEuler(float yaw, float pitch, float roll) {
	float cosy, siny, cosp, sinp, cosr, sinr;
	mtrx3 rt;

	cosy = cosf(degToRad(yaw));
	siny = sinf(degToRad(yaw));
	cosp = cosf(degToRad(pitch));
	sinp = sinf(degToRad(pitch));
	cosr = cosf(degToRad(roll));
	sinr = sinf(degToRad(roll));
	
	rt[0]  = cosy*cosr - siny*cosp*sinr;
	rt[1]  = -cosy*sinr - siny*cosp*cosr;
	rt[2]  = siny * sinp;

	rt[3]  = siny*cosr + cosy*cosp*sinr;
	rt[4]  = -siny*sinr + cosy*cosp*cosr;
	rt[5]  = -cosy * sinp;

	rt[6]  = sinp * sinr;
	rt[7]  = sinp * cosr;
	rt[8]  = cosp;

	return rt;
}

mtrx3 mtrx3FromAxisAngl(const vec3 &ax, float phi) {
	float cosphi, sinphi, vxvy, vxvz, vyvz, vx, vy, vz;
	mtrx3 rt;

	cosphi = cosf(degToRad(phi));
	sinphi = sinf(degToRad(phi));
	vxvy = ax[_XC] * ax[_YC];
	vxvz = ax[_XC] * ax[_ZC];
	vyvz = ax[_YC] * ax[_ZC];
	vx = ax[_XC];
	vy = ax[_YC];
	vz = ax[_ZC];

	rt[0]  = cosphi + (1.0-cosphi)*vx*vx;
	rt[1]  = (1.0-cosphi)*vxvy - sinphi*vz;
	rt[2]  = (1.0-cosphi)*vxvz + sinphi*vy;

	rt[3]  = (1.0-cosphi)*vxvy + sinphi*vz;
	rt[4]  = cosphi + (1.0-cosphi)*vy*vy;
	rt[5]  = (1.0-cosphi)*vyvz - sinphi*vx;

	rt[6]  = (1.0-cosphi)*vxvz - sinphi*vy;
	rt[7]  = (1.0-cosphi)*vyvz + sinphi*vx;
	rt[8]  = cosphi + (1.0-cosphi)*vz*vz;

	return rt;
}

mtrx3 mtrx3FromOffset(const vec2 &src) {
	mtrx3 rt;

	rt[2]  = src[0];
    rt[5]  = src[1];
    rt[8]  = 1.0f;

	return rt;
}

mtrx3 mtrx3FromScale(const vec2 &src) {
	mtrx3 rt;

	rt[0]  = src[0];
    rt[4]  = src[1];
    rt[8]  = 1.0f;

	return rt;
}

mtrx3 mtrx3FromQtnn(const qtnn &src) {
	mtrx3 rt;
	float wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;

    x2 = src[0] + src[0];
    y2 = src[1] + src[1];
    z2 = src[2] + src[2];
    xx = src[0] * x2;   xy = src[0] * y2;   xz = src[0] * z2;
    yy = src[1] * y2;   yz = src[1] * z2;   zz = src[2] * z2;
    wx = src[3] * x2;   wy = src[3] * y2;   wz = src[3] * z2;

    rt[0]=1.0-(yy+zz);  rt[1]=xy-wz;        rt[ 2]=xz+wy;       
    rt[3]=xy+wz;        rt[4]=1.0-(xx+zz);  rt[ 5]=yz-wx;       
    rt[6]=xz-wy;        rt[7]=yz+wx;        rt[ 8]=1.0-(xx+yy); 

    return rt;
}

mtrx3 mtrx3SetYaw(float angl) {
	float sa, ca;
	mtrx3 rt;

	sa = sinf(degToRad(angl));
	ca = cosf(degToRad(angl));

	rt[0] = ca;   rt[1] = -sa;  rt[2] = 0.0f;
	rt[3] = sa;   rt[4] =  ca;  rt[5] = 0.0f;
	rt[6] = 0.0f; rt[7] = 0.0f; rt[8] = 1.0f;

	return rt;
}

mtrx3 mtrx3SetPitch(float angl) {
	float sa, ca;
	mtrx3 rt;

	sa = sinf(degToRad(angl));
	ca = cosf(degToRad(angl));

	rt[0] = 1.0f; rt[1] = 0.0f; rt[2] = 0.0f;
	rt[3] = 0.0f; rt[4] = ca;   rt[5] = -sa;
	rt[6] = 0.0f; rt[7] = sa;   rt[8] = ca;

	return rt;
}

mtrx3 mtrx3SetRoll(float angl)
{
	float sa, ca;
	mtrx3 rt;

	sa = sinf(degToRad(angl));
	ca = cosf(degToRad(angl));

	rt[0] = ca;   rt[1] = 0.0f; rt[2] = sa;
	rt[3] = 0.0f; rt[4] = 1.0f; rt[5] = 0.0f;
	rt[6] = -sa;  rt[7] = 0.0f; rt[8] = ca;

	return rt;
}

float mtrx3Det(mtrx3 m) {
	return m[0]*m[4]*m[8] +
		   m[6]*m[1]*m[5] +
		   m[2]*m[3]*m[7] -
		   m[0]*m[7]*m[5] -
		   m[8]*m[3]*m[1];
}

float mtrx3_det_lu(mtrx3 m) {
	mtrx3 l, u;
	tuple<mtrx3, mtrx3> lu;
	float 	l_det, u_det;
	
	lu = mtrx3LU(m);

	l_det = get<0>(lu)[0];
	u_det = get<1>(lu)[0];

	for (size_t i = 1; i < mrange; i++) {
		l_det *= l[idRw(i, i, mrange)];
		u_det *= u[idRw(i, i, mrange)];
	}

	return l_det * u_det;
}

mtrx3 mtrx3Mult(mtrx3 a, mtrx3 b) {
    mtrx3 rt = {0.0f};

	for (size_t i = 0; i < mrange; i++) {
		for (size_t j = 0; j < mrange; j++) {
			for (size_t k = 0; k < mrange; k++) {
				rt[idRw(i, j, mrange)] += a[idRw(k, j, mrange)]*b[idRw(i, k, mrange)];
			}
		}
	}

	return rt;
}

vec3 mtrx3MultVec(mtrx3 m, vec3 v) {
	vec3	rt;

	for (size_t i = 0; i < mrange; i++) {
		for (size_t  j = 0; j < mrange; j++) {
			rt[i] += m[idRw(i, j, mrange)]*v[j];
		}
	}

	return rt;
}

/*
	Нижнетреугольная (L, lm) матрица имеет единицы по диагонали
*/

tuple<mtrx3, mtrx3> mtrx3LU(mtrx3 m) {
	mtrx3 lm, um; 
	size_t i, j, k;
	float sum;    

	for (i = 0; i < mrange; i++) {
		for (k = i; k < mrange; k++) {
			sum = 0;
			for (j = 0; j < i; j++) {
				sum += (lm[idRw(i, j, mrange)] * um[idRw(j, k, mrange)]);
			}
			um[idRw(i, k, mrange)] = m[idRw(i, k, mrange)] - sum;
		}

		for (k = i; k < mrange; k++) {
			if (i == k) {
				lm[idRw(i, i, mrange)] = 1.0;
			} else {
				sum = 0;
				for (j = 0; j < i; j++) {
					sum += lm[idRw(k, j, mrange)] * um[idRw(j, i, mrange)];
				}
				lm[idRw(k, i, mrange)] = (m[idRw(k, i, mrange)] - sum) / um[idRw(i, i, mrange)];
			}
		}
	}

	return {lm, um};
}

tuple<mtrx3, vec3> mtrx3LDLT(mtrx3 m) {
	mtrx3 lm;
	vec3 dv;
	size_t i, j, k;
	float sum;   

	for (i = 0; i < mrange; i++) {
		for (j = i; j < mrange; j++) {
			sum = m[idRw(j, i, mrange)];
			for (k = 0; k < i; k++) {
				sum = sum - lm[idRw(i, k, mrange)]*dv[k]*lm[idRw(j, k, mrange)];
				if (i == j) {
					if (sum <= 0) {
						std::cout << "mtrx3_ldlt(): matrix is not positive deﬁnite\n";
						return {mtrx3FromIdtt(), vec3(0.0f, 0.0f, 0.0f)};
					}
					dv[i] = sum;
					lm[idRw(i, i, mrange)] = 1.0;
				} else {
					lm[idRw(j, i, mrange)] = sum / dv[i];
				}
			}
		}
	}

	return {lm, dv};
}

mtrx3 mtrx3GetTranspose(mtrx3 m) {
	float tmp;
	mtrx3 rt;

	rt = m;

	for (size_t i = 0; i < mrange; i++) {
		for (size_t j = 0; j < i; j++) {
			tmp = rt[idRw(i, i, mrange)];
			rt[idRw(i, j, mrange)] = rt[idRw(j, i, mrange)];
			rt[idRw(j, i, mrange)] = tmp;
		}
	}

	return rt;
}

mtrx3 mtrx3GetInvert(mtrx3 m) {
	mtrx3 inverse, rt;
	float det, invDet;

	inverse[0] = m[4]*m[8] - m[5]*m[7];
	inverse[3] = m[5]*m[6] - m[3]*m[8];
	inverse[6] = m[3]*m[7] - m[4]*m[6];

	det = m[0]*inverse[0] + m[1]*inverse[3] +
		m[2]*inverse[6];

	if (fabs(det) < f_eps) {
		std::cout << "mtrx3_invert(): determinant is a zero!\n";
		return mtrx3();
	}

	invDet = 1.0f / det;

	inverse[1] = m[2]*m[7] - m[1]*m[8];
	inverse[2] = m[1]*m[5] - m[2]*m[4];
	inverse[4] = m[0]*m[8] - m[2]*m[6];
	inverse[5] = m[2]*m[3] - m[0]*m[5];
	inverse[7] = m[1]*m[6] - m[0]*m[7];
	inverse[8] = m[0]*m[4] - m[1]*m[3];

	rt[0] = inverse[0] * invDet;
	rt[1] = inverse[1] * invDet;
	rt[2] = inverse[2] * invDet;

	rt[3] = inverse[3] * invDet;
	rt[4] = inverse[4] * invDet;
	rt[5] = inverse[5] * invDet;

	rt[6] = inverse[6] * invDet;
	rt[7] = inverse[7] * invDet;
	rt[8] = inverse[8] * invDet;

	return rt;
}

vec3 mtrx3SolveGauss(mtrx3 m, vec3 v) {
	size_t i, j, k;
	// float a[mrange][mrange + 1];
	float t;
	std::array<std::array<float, mrange+1>, mrange> a;
	vec3 rt;

	for (i = 0; i < mrange; i++) { //было ++i
		for (j = 0; j < mrange; j++) { //было ++j
			a[i][j] = m[idRw(i, j, mrange)];
			a[i][mrange] = v[i];
		}
	}

	/* Pivotisation */
	for (i = 0; i < mrange; i++) {
		for (k = i + 1; k < mrange; k++) {
			if (fabs(a[i][i]) < fabs(a[k][i])) {
				for (j = 0; j <= mrange; j++) {
					t = a[i][j];
					a[i][j] = a[k][j];
					a[k][j] = t;
				}
			}
		}
	}

	/* прямой ход */
	for (k = 1; k < mrange; k++) {
		for (j = k; j < mrange; j++) {
			t = a[j][k-1] / a[k-1][k-1];
			for (i = 0; i < mrange+1; i++) {
				a[j][i] = a[j][i] - t*a[k-1][i];
			}
		}
	}

	/* обратный ход */
	for (i = mrange - 1; i > 0; i--) {
		rt[i] = a[i][mrange] / a[i][i];
		for (j = mrange - 1; j > i; j--) {
			rt[i] = rt[i] - a[i][j]*rt[j]/a[i][i];
		}
	}

	return rt;
}

mtrx3 mtrx3InsertCmn(mtrx3 m, vec3 v, int cmn) {
	size_t i, j = 0;
	mtrx3 rt;

	rt = m;

	for (i = cmn; i < mrange*mrange; i += mrange) {
		rt[i] = v[j];
		j++;
	}

	return rt;
}

vec3 mtrx3SolveKramer(mtrx3 m, vec3 v) {
	float det;
	mtrx3 kr_mtrx;
	vec3 rt;

	det = mtrx3Det(m);

	if (fabs(det) < f_eps) {
		printf("mtrx3_solve_kramer(): system has no solve");
		return vec3(0.0, 0.0, 0.0);
	}

	for (size_t i = 0; i < mrange; i++) {
		kr_mtrx = mtrx3InsertCmn(m, v, i);
		rt[i] = mtrx3Det(kr_mtrx) / det;
	}

	return rt;
}
