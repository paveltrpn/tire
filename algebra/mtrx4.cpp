
#include <iostream>
#include <tuple>
#include <cmath>

#include "mtrx4.h"
#include "vec4.h"

using namespace std;

float mtrx4DetLU(mtrx4 m) {
	constexpr int mrange = 4;
	int		i;         
	mtrx4 l, u;
	tuple<mtrx4, mtrx4> lu;
	float 	l_det, u_det;
	
	lu = mtrx4LU(m);

	l_det = get<0>(lu)[0];
	u_det = get<1>(lu)[0];

	for (i = 1; i < mrange; i++) {
		l_det *= l[idRw(i, i, mrange)];
		u_det *= u[idRw(i, i, mrange)];
	}

	return l_det * u_det;
}

mtrx4 mtrx4Mult(mtrx4 a, mtrx4 b) {
	constexpr int mrange = 4;
	int i, j, k;
	float tmp;
    mtrx4 rt;

	for (i = 0; i < mrange; i++) {
		for (j = 0; j < mrange; j++) {
			tmp = 0.0;
			for (k = 0; k < mrange; k++) {
				tmp = tmp + a[idRw(k, j, mrange)]*b[idRw(i, k, mrange)];
			}
			rt[idRw(i, j, mrange)] = tmp;
		}
	}

	return rt;
}

vec4 mtrx4MultVec(mtrx4 m, vec4 v) {
	constexpr int mrange = 4;
	int		i, j;
	float	tmp;
	vec4 rt;

	for (i = 0; i < mrange; i++) {
		tmp = 0;
		for (j = 0; j < mrange; j++) {
			tmp = tmp + m[idRw(i, j, mrange)]*v[j];
		}
		rt[i] = tmp;
	}

	return rt;
}

vec3 mtrx4MultVec3(mtrx4 m, vec3 v) {
	vec3 rt;
	vec4 tmp, mv = {v[0], v[1], v[2], 1.0f};

	tmp = mtrx4MultVec(m, mv);
	
	return vec3(tmp[0], tmp[1], tmp[2]);
}

/*
	Нижнетреугольная (L, lm) матрица имеет единицы по диагонали
*/

tuple<mtrx4, mtrx4> mtrx4LU(mtrx4 m) {
	constexpr int mrange = 4;
	mtrx4 lm, um; 
	int i, j, k;
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

tuple<mtrx4, vec4> mtrx4LDLT(mtrx4 m) {
	constexpr int mrange = 4;
	mtrx4 lm;
	vec4 dv;
	int i, j, k;
	float sum;   

	for (i = 0; i < mrange; i++) {
		for (j = i; j < mrange; j++) {
			sum = m[idRw(j, i, mrange)];
			for (k = 0; k < i; k++) {
				sum = sum - lm[idRw(i, k, mrange)]*dv[k]*lm[idRw(j, k, mrange)];
				if (i == j) {
					if (sum <= 0) {
						printf("mtrx4LDLT(): matrix is not positive deﬁnite");
						return {mtrx4(), vec4(0.0, 0.0, 0.0, 0.0)};
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

mtrx4 mtrx4Transpose(mtrx4 m) {
	constexpr int mrange = 4;
	int i, j;
	float tmp;
	mtrx4 rt;

	rt = m;

	for (i = 0; i < mrange; i++) {
		for (j = 0; j < i; j++) {
			tmp = rt[idRw(i, i, mrange)];
			rt[idRw(i, j, mrange)] = rt[idRw(j, i, mrange)];
			rt[idRw(j, i, mrange)] = tmp;
		}
	}

	return rt;
}

vec4 mtrx4SolveGauss(mtrx4 m, vec4 v) {
	constexpr int mrange = 4;
	int i, j, k;
	float a[mrange][mrange + 1], t;
    vec4 rt;

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
	for (i = mrange - 1; i >= 0; i--) {
		rt[i] = a[i][mrange] / a[i][i];
		for (j = mrange - 1; j > i; j--) {
			rt[i] = rt[i] - a[i][j]*rt[j]/a[i][i];
		}
	}

	return rt;
}

mtrx4 mtrx4InsertCmn(mtrx4 m, vec4 v, int cmn) {
	constexpr int mrange = 4;
	int i, j = 0;
	mtrx4 rt;

	rt = m;

	for (i = cmn; i < mrange*mrange; i += mrange) {
		rt[i] = v[j];
		j++;
	}

	return rt;
}

vec4 mtrx4SolveKramer(mtrx4 m, vec4 v) {
	constexpr int mrange = 3;
	int i;
	float det;
	mtrx4 kr_mtrx;
    vec4 rt;

	det = mtrx4DetLU(m);

	if (fabs(det) < f_eps) {
		printf("mtrx4SolveKramer(): system has no solve");
		return vec4(0.0, 0.0, 0.0, 0.0);
	}

	for (i = 0; i < mrange; i++) {
		kr_mtrx = mtrx4InsertCmn(m, v, i);
		rt[i] = mtrx4DetLU(kr_mtrx) / det;
	}

	return rt;
}
