
#include <iostream>
#include <tuple>
#include <cmath>

#include "mtrx4.h"

using namespace std;

mtrx4_t::mtrx4_t(float yaw, float pitch, float roll) {
	float cosy, siny, cosp, sinp, cosr, sinr;
	
	cosy = cosf(deg_to_rad(yaw));
	siny = sinf(deg_to_rad(yaw));
	cosp = cosf(deg_to_rad(pitch));
	sinp = sinf(deg_to_rad(pitch));
	cosr = cosf(deg_to_rad(roll));
	sinr = sinf(deg_to_rad(roll));

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
}

mtrx4_t::mtrx4_t(const vec3_t &ax, float phi) {
	float cosphi, sinphi, vxvy, vxvz, vyvz, vx, vy, vz;

	cosphi = cosf(deg_to_rad(phi));
	sinphi = sinf(deg_to_rad(phi));
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
}

mtrx4_t mtrx4_idtt() {
	mtrx4_t rt;
    constexpr int mrange = 4;
	int i, j;

	for (i = 0; i < mrange; i++) {
		for (j = 0; j < mrange; j++) {
			if (i == j) {
				rt[id_rw(i, j, mrange)] = 1.0f;
			} else {
				rt[id_rw(i, j, mrange)] = 0.0f;
			}
		}
	}

	return rt;
}

mtrx4_t mtrx4_set(float m[16]) {
    mtrx4_t rt;
	constexpr int mrange = 4;
	int	i, j;

	for (i = 0; i < mrange; i++) {
		for (j = 0; j < mrange; j++) {
			rt[id_rw(i, j, mrange)] = m[id_rw(i, j, mrange)];
		}
	}

	return rt;
}

mtrx4_t mtrx4_set_float(float a00, float a01, float a02, float a03,
	                    float a10, float a11, float a12, float a13,
	                    float a20, float a21, float a22, float a23,
	                    float a30, float a31, float a32, float a33) {
	mtrx4_t rt;

	rt[0] = a00;
	rt[1] = a01;
	rt[2] = a02;
	rt[3] = a03;

	rt[4] = a10;
	rt[5] = a11;
	rt[6] = a12;
	rt[7] = a13;

	rt[8] = a20;
	rt[9] = a21;
	rt[10] = a22;
	rt[11] = a23;

	rt[12] = a30;
	rt[13] = a31;
	rt[14] = a32;
	rt[15] = a33;

	return rt;
}

void mtrx4_show(mtrx4_t m) {
	printf("%5.2f %5.2f %5.2f %5.2f\n", m[0], m[1], m[2], m[3]);
	printf("%5.2f %5.2f %5.2f %5.2f\n", m[4], m[5], m[6], m[7]);
	printf("%5.2f %5.2f %5.2f %5.2f\n", m[8], m[9], m[10], m[11]);
	printf("%5.2f %5.2f %5.2f %5.2f\n", m[12], m[13], m[14], m[15]);
}

float mtrx4_det_lu(mtrx4_t m) {
	constexpr int mrange = 4;
	int		i;         
	mtrx4_t l, u;
	tuple<mtrx4_t, mtrx4_t> lu;
	float 	l_det, u_det;
	
	lu = mtrx4_lu(m);

	l_det = get<0>(lu)[0];
	u_det = get<1>(lu)[0];

	for (i = 1; i < mrange; i++) {
		l_det *= l[id_rw(i, i, mrange)];
		u_det *= u[id_rw(i, i, mrange)];
	}

	return l_det * u_det;
}

mtrx4_t mtrx4_mult(mtrx4_t a, mtrx4_t b) {
	constexpr int mrange = 4;
	int i, j, k;
	float tmp;
    mtrx4_t rt;

	for (i = 0; i < mrange; i++) {
		for (j = 0; j < mrange; j++) {
			tmp = 0.0;
			for (k = 0; k < mrange; k++) {
				tmp = tmp + a[id_rw(k, j, mrange)]*b[id_rw(i, k, mrange)];
			}
			rt[id_rw(i, j, mrange)] = tmp;
		}
	}

	return rt;
}

vec4_t mtrx4_mult_vec(mtrx4_t m, vec3_t v) {
	constexpr int mrange = 4;
	int		i, j;
	float	tmp;
	vec4_t rt;

	for (i = 0; i < mrange; i++) {
		tmp = 0;
		for (j = 0; j < mrange; j++) {
			tmp = tmp + m[id_rw(i, j, mrange)]*v[j];
		}
		rt[i] = tmp;
	}

	return rt;
}

/*
	Нижнетреугольная (L, lm) матрица имеет единицы по диагонали
*/

tuple<mtrx4_t, mtrx4_t> mtrx4_lu(mtrx4_t m) {
	constexpr int mrange = 4;
	mtrx4_t lm, um; 
	int i, j, k;
	float sum;    

	for (i = 0; i < mrange; i++) {
		for (k = i; k < mrange; k++) {
			sum = 0;
			for (j = 0; j < i; j++) {
				sum += (lm[id_rw(i, j, mrange)] * um[id_rw(j, k, mrange)]);
			}
			um[id_rw(i, k, mrange)] = m[id_rw(i, k, mrange)] - sum;
		}

		for (k = i; k < mrange; k++) {
			if (i == k) {
				lm[id_rw(i, i, mrange)] = 1.0;
			} else {
				sum = 0;
				for (j = 0; j < i; j++) {
					sum += lm[id_rw(k, j, mrange)] * um[id_rw(j, i, mrange)];
				}
				lm[id_rw(k, i, mrange)] = (m[id_rw(k, i, mrange)] - sum) / um[id_rw(i, i, mrange)];
			}
		}
	}

	return {lm, um};
}

tuple<mtrx4_t, vec4_t> mtrx4_ldlt(mtrx4_t m) {
	constexpr int mrange = 4;
	mtrx4_t lm;
	vec4_t dv;
	int i, j, k;
	float sum;   

	for (i = 0; i < mrange; i++) {
		for (j = i; j < mrange; j++) {
			sum = m[id_rw(j, i, mrange)];
			for (k = 0; k < i; k++) {
				sum = sum - lm[id_rw(i, k, mrange)]*dv[k]*lm[id_rw(j, k, mrange)];
				if (i == j) {
					if (sum <= 0) {
						printf("mtrx4_ldlt(): matrix is not positive deﬁnite");
						return {mtrx4_idtt(), vec4_t(0.0, 0.0, 0.0, 0.0)};
					}
					dv[i] = sum;
					lm[id_rw(i, i, mrange)] = 1.0;
				} else {
					lm[id_rw(j, i, mrange)] = sum / dv[i];
				}
			}
		}
	}

	return {lm, dv};
}

mtrx4_t mtrx4_transpose(mtrx4_t m) {
	constexpr int mrange = 3;
	int i, j;
	float tmp;
	mtrx4_t rt;

	rt = m;

	for (i = 0; i < mrange; i++) {
		for (j = 0; j < i; j++) {
			tmp = rt[id_rw(i, i, mrange)];
			rt[id_rw(i, j, mrange)] = rt[id_rw(j, i, mrange)];
			rt[id_rw(j, i, mrange)] = tmp;
		}
	}

	return rt;
}

vec4_t mtrx4_solve_gauss(mtrx4_t m, vec4_t v) {
	constexpr int mrange = 4;
	int i, j, k;
	float a[mrange][mrange + 1], t;
    vec4_t rt;

	for (i = 0; i < mrange; i++) { //было ++i
		for (j = 0; j < mrange; j++) { //было ++j
			a[i][j] = m[id_rw(i, j, mrange)];
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
	for (i = mrange - 1; i >= 0; i--) {
		rt[i] = a[i][mrange] / a[i][i];
		for (j = mrange - 1; j > i; j--) {
			rt[i] = rt[i] - a[i][j]*rt[j]/a[i][i];
		}
	}

	return rt;
}

mtrx4_t mtrx4_insert_cmn(mtrx4_t m, vec4_t v, int cmn) {
	constexpr int mrange = 4;
	int i, j = 0;
	mtrx4_t rt;

	rt = m;

	for (i = cmn; i < mrange*mrange; i += mrange) {
		rt[i] = v[j];
		j++;
	}

	return rt;
}

vec4_t mtrx4_solve_kramer(mtrx4_t m, vec4_t v) {
	constexpr int mrange = 3;
	int i;
	float det;
	mtrx4_t kr_mtrx;
    vec4_t rt;

	det = mtrx4_det_lu(m);

	if (fabs(det) < f_eps) {
		printf("mtrx4_solve_kramer(): system has no solve");
		return vec4_t(0.0, 0.0, 0.0, 0.0);
	}

	for (i = 0; i < mrange; i++) {
		kr_mtrx = mtrx4_insert_cmn(m, v, i);
		rt[i] = mtrx4_det_lu(kr_mtrx) / det;
	}

	return rt;
}
