
#include <iostream>
#include <tuple>
#include <cmath>

#include "mtrx2.h"

using namespace std;

mtrx2_t::mtrx2_t(float phi) {
	float sinphi = sinf(deg_to_rad(phi));
	float cosphi = cosf(deg_to_rad(phi));

	data[0] = cosphi;
	data[1] = -sinphi;
	data[2] = -sinphi;
	data[3] = cosphi;
}

mtrx2_t mtrx2_idtt() {
	mtrx2_t rt;
    constexpr int mrange = 2;
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

mtrx2_t mtrx2_set(float m[4]) {
    mtrx2_t rt;
	constexpr int mrange = 2;
	int	i, j;

	for (i = 0; i < mrange; i++) {
		for (j = 0; j < mrange; j++) {
			rt[id_rw(i, j, mrange)] = m[id_rw(i, j, mrange)];
		}
	}

	return rt;
}

mtrx2_t mtrx2_set_float(float a00, float a01, float a10, float a11) {
    mtrx2_t rt;

	rt[0] = a00;
	rt[1] = a01;
	rt[2] = a10;
	rt[3] = a11;

	return rt;
}

mtrx2_t mtrx2_rtn(float phi) {
	mtrx2_t rt;

	float cosphi, sinphi;
	
	sinphi = sinf(phi);
	cosphi = cosf(phi);

	rt[0] = cosphi;
	rt[1] = -sinphi;
	rt[2] = -sinphi;
	rt[3] = cosphi;

	return rt;
}

void mtrx2_show(mtrx2_t m) {
	printf("%5.2f %5.2f\n", m[0], m[1]);
	printf("%5.2f %5.2f\n", m[2], m[3]);
}

float mtrx2_det(mtrx2_t m) {
	return m[0]*m[3] - m[1]*m[2];
}

float mtrx2_det_lu(mtrx2_t m) {
	constexpr int mrange = 2;
	int		i;         
	mtrx2_t l, u;
	tuple<mtrx2_t, mtrx2_t> lu;
	float 	l_det, u_det;
	
	lu = mtrx2_lu(m);

	l_det = get<0>(lu)[0];
	u_det = get<1>(lu)[0];

	for (i = 1; i < mrange; i++) {
		l_det *= l[id_rw(i, i, mrange)];
		u_det *= u[id_rw(i, i, mrange)];
	}

	return l_det * u_det;
}

mtrx2_t mtrx2_mult(mtrx2_t a, mtrx2_t b) {
	constexpr int mrange = 2;
	int i, j, k;
	float tmp;
	mtrx2_t rt;

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

vec2_t mtrx2_mult_vec(mtrx2_t m, vec2_t v) {
	constexpr int mrange = 2;
	vec2_t rt;

	int		i, j;
	float	tmp;

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

tuple<mtrx2_t, mtrx2_t> mtrx2_lu(mtrx2_t m) {
	constexpr int mrange = 2;
	mtrx2_t lm, um; 
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

tuple<mtrx2_t, vec2_t> mtrx2_ldlt(mtrx2_t m) {
	constexpr int mrange = 2;
	mtrx2_t lm;
	vec2_t dv;
	int i, j, k;
	float sum;   

	for (i = 0; i < mrange; i++) {
		for (j = i; j < mrange; j++) {
			sum = m[id_rw(j, i, mrange)];
			for (k = 0; k < i; k++) {
				sum = sum - lm[id_rw(i, k, mrange)]*dv[k]*lm[id_rw(j, k, mrange)];
				if (i == j) {
					if (sum <= 0) {
						printf("mtrx2_ldlt(): matrix is not positive deﬁnite");
						return {mtrx2_idtt(), vec2_t(0.0, 0.0)};
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

mtrx2_t mtrx2_transpose(mtrx2_t m) {
	constexpr int mrange = 2;
	int i, j;
	float tmp;
	mtrx2_t rt;

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

mtrx2_t mtrx2_invert(mtrx2_t m) {
	mtrx2_t rt;
	float det;
	
	det = mtrx2_det(m);

	if (fabs(det) < f_eps) {
		printf("mtrx2_invert(): determinant is a zero!");
		return mtrx2_idtt();
	}

	return mtrx2_t(m[3], -m[1]/det, -m[2]/det, m[0]/det);
}

vec2_t mtrx2_solve_gauss(mtrx2_t m, vec2_t v) {
	constexpr int mrange = 2;
	int i, j, k;
	float a[mrange][mrange + 1], t;
	vec2_t rt;

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

mtrx2_t mtrx2_insert_cmn(mtrx2_t m, vec2_t v, int cmn) {
	constexpr int mrange = 2;
	int i, j = 0;
	mtrx2_t rt;

	rt = m;

	for (i = cmn; i < mrange*mrange; i += mrange) {
		rt[i] = v[j];
		j++;
	}

	return rt;
}

vec2_t mtrx2_solve_kramer(mtrx2_t m, vec2_t v) {
	constexpr int mrange = 2;
	int i;
	float det;
	mtrx2_t kr_mtrx;
	vec2_t rt;

	det = mtrx2_det(m);

	if (fabs(det) < f_eps) {
		printf("mtrx2_solve_kramer(): system has no solve");
		return vec2_t(0.0, 0.0);
	}

	for (i = 0; i < mrange; i++) {
		kr_mtrx = mtrx2_insert_cmn(m, v, i);
		rt[i] = mtrx2_det(kr_mtrx) / det;
	}

	return rt;
}
