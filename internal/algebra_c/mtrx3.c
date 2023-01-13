
#include <stdio.h>
#include <math.h>

#include "algebra_c.h"

void mtrx3Copy(const mtrx3 m, mtrx3 rt) {
	int32_t i, j;

	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			rt[idRw(i, j, 3)] = m[idRw(i, j, 3)];
		}
	}
}

void mtrx3Set(float a00, float a01, float a02,
              float a10, float a11, float a12,
              float a20, float a21, float a22,
              mtrx3 rt) {
	rt[0] = a00;
	rt[1] = a01;
	rt[2] = a02;

	rt[3] = a10;
	rt[4] = a11;
	rt[5] = a12;

	rt[6] = a20;
	rt[7] = a21;
	rt[8] = a22;
}

void mtrx3Zero(mtrx3 m) {
	for (int i = 0; i < (3*3); i++) {
		m[i] = 0.0f;
	}
}

void mtrx3SetEuler(float yaw, float pitch, float roll, mtrx3 rt) {
	float cosy, siny, cosp, sinp, cosr, sinr;
	
	cosy = cosf(yaw);
	siny = sinf(yaw);
	cosp = cosf(pitch);
	sinp = sinf(pitch);
	cosr = cosf(roll);
	sinr = sinf(roll);

	rt[0] = cosy*cosr - siny*cosp*sinr;
	rt[1] = -cosy*sinr - siny*cosp*cosr;
	rt[2] = siny * sinp;

	rt[3] = siny*cosr + cosy*cosp*sinr;
	rt[4] = -siny*sinr + cosy*cosp*cosr;
	rt[5] = -cosy * sinp;

	rt[6] = sinp * sinr;
	rt[7] = sinp * cosr;
	rt[8] = cosp;
}

void mtrx3SetAxisAngl(const vec3 ax, float phi, mtrx3 rt) {
	float cosphi, sinphi, vxvy, vxvz, vyvz, vx, vy, vz;

	cosphi = cosf(phi);
	sinphi = sinf(phi);
	vxvy = ax[0] * ax[1];
	vxvz = ax[0] * ax[2];
	vyvz = ax[1] * ax[2];
	vx = ax[0];
	vy = ax[1];
	vz = ax[2];

	rt[0] = cosphi + (1.0-cosphi)*vx*vx;
	rt[1] = (1.0-cosphi)*vxvy - sinphi*vz;
	rt[2] = (1.0-cosphi)*vxvz + sinphi*vy;

	rt[3] = (1.0-cosphi)*vxvy + sinphi*vz;
	rt[4] = cosphi + (1.0-cosphi)*vy*vy;
	rt[5] = (1.0-cosphi)*vyvz - sinphi*vz;

	rt[6] = (1.0-cosphi)*vxvz - sinphi*vy;
	rt[7] = (1.0-cosphi)*vyvz + sinphi*vx;
	rt[8] = cosphi + (1.0-cosphi)*vz*vz;
}

void mtrx3Idtt(mtrx3 rt) {
	int32_t i, j;

	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			if (i == j) {
				rt[idRw(i, j, 3)] = 1.0f;
			} else {
				rt[idRw(i, j, 3)] = 0.0f;
			}
		}
	}
}

float mtrx3Det(const mtrx3 m) {
	return m[0]*m[4]*m[8] +
		   m[6]*m[1]*m[5] +
		   m[2]*m[3]*m[7] -
		   m[0]*m[7]*m[5] -
		   m[8]*m[3]*m[1];
}

void mtrx3Mult(const mtrx3 a, const mtrx3 b, mtrx3 rt) {
	int32_t i, j;
	
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			rt[idRw(i, j, 3)] =
				a[idRw(0, j, 3)]*b[idRw(i, 0, 3)] +
					a[idRw(1, j, 3)]*b[idRw(i, 1, 3)] +
					a[idRw(2, j, 3)]*b[idRw(i, 2, 3)];
		}
	}
}

void mtrx3MultVec3(const mtrx3 m, const vec3 v, vec3 rt) {
	rt[0] = m[0]*v[0] + m[1]*v[1] + m[2]*v[2];
	rt[1] = m[3]*v[0] + m[4]*v[1] + m[5]*v[2];
	rt[2] = m[6]*v[0] + m[7]*v[1] + m[8]*v[2];
}

/*
	Где-то здесь ошибка, долго искал
	ничего не вышло и взял код из сети
*/
/*
func mtrx3_lu(m mtrx3) (l, u mtrx3) {
	var (
		i, j, k int32
		lm, um  mtrx3
		sum     float32
	)

	for j = 0; j < 3; j++ {
		um[idRw(0, j, 3)] = m[idRw(0, j, 3)]
	}

	for j = 0; j < 3; j++ {
		lm[idRw(j, 0, 3)] = m[idRw(j, 0, 3)] / um[idRw(0, 0, 3)]
	}

	for i = 1; i < 3; i++ {
		for j = i; j < 3; j++ {
			sum = 0.0
			for k = 0; k < i; k++ {
				sum = sum + lm[idRw(i, k, 3)]*um[idRw(k, j, 3)]
			}
			um[idRw(i, j, 3)] = m[idRw(i, j, 3)] - sum
		}
	}

	for i = 1; i < 3; i++ {
		for j = i; j < 3; j++ {
			if i > j {
				lm[idRw(j, i, 3)] = 0.0
			} else {
				sum = 0.0
				for k = 0; k < i; k++ {
					sum = sum + lm[idRw(j, k, 3)]*um[idRw(k, i, 3)]
				}
				lm[idRw(j, i, 3)] = (1.0 / um[idRw(i, i, 3)]) * (m[idRw(j, i, 3)] - sum)
			}
		}
	}

	return lm, um
}
*/

/*
	Нижнетреугольная (L, lm) матрица имеет единицы по диагонали
*/
void mtrx3LU(const mtrx3 m, mtrx3 lm, mtrx3 um) {
	int32_t	i, j, k; 
	float sum = 0;

	for (i = 0; i < 3; i++) {
		for (k = i; k < 3; k++) {
			sum = 0;
			for (j = 0; j < i; j++) {
				sum += (lm[idRw(i, j, 3)] * um[idRw(j, k, 3)]);
			}
			um[idRw(i, k, 3)] = m[idRw(i, k, 3)] - sum;
		}

		for (k = i; k < 3; k++) {
			if (i == k) {
				lm[idRw(i, i, 3)] = 1.0;
			} else {
				sum = 0;
				for (j = 0; j < i; j++) {
					sum += lm[idRw(k, j, 3)] * um[idRw(j, i, 3)];
				}
				lm[idRw(k, i, 3)] = (m[idRw(k, i, 3)] - sum) / um[idRw(i, i, 3)];
			}
		}
	}
}

 void mtrx3LDLT(const mtrx3 m, mtrx3 lm, vec3 dv) {
	int32_t	i, j, k; 
	float sum;

	for (i = 0; i < 3; i++) {
		for (j = i; j < 3; j++) {
			sum = m[idRw(j, i, 3)];
			for (k = 0; k < i; k++) {
				sum = sum - lm[idRw(i, k, 3)]*dv[k]*lm[idRw(j, k, 3)];
				if (i == j) {
					if (sum <= 0) {
						printf("mtrx3LDLT(): matrix is not positive definite \n");
						mtrx3Idtt(lm);
						vec3Zero(dv);
						return;
					}
					dv[i] = sum;
					lm[idRw(i, i, 3)] = 1.0;
				} else {
					lm[idRw(j, i, 3)] = sum / dv[i];
				}
			}
		}
	}
}

void mtrx3GetTranspose(const mtrx3 m, mtrx3 rt) {
	int32_t i, j;
	float tmp;

	mtrx3Copy(m, rt);

	for (i = 0; i < 3; i++) {
		for (j = 0; j < i; j++) {
			tmp = rt[idRw(i, i, 3)];
			rt[idRw(i, j, 3)] = rt[idRw(j, i, 3)];
			rt[idRw(j, i, 3)] = tmp;
		}
	}
}

void mtrx3TranposeSelf(mtrx3 m) {
	int32_t i, j;
	float tmp;

	for (i = 0; i < 3; i++) {
		for (j = 0; j < i; j++) {
			tmp = m[idRw(i, i, 3)];
			m[idRw(i, j, 3)] = m[idRw(j, i, 3)];
			m[idRw(j, i, 3)] = tmp;
		}
	}
}

void mtrx3GetInv(const mtrx3 m, mtrx3 rt) {
	mtrx3 inverse;
	float det, invDet;

	inverse[idRw(0, 0, 3)] = m[idRw(1, 1, 3)] * m[idRw(2, 2, 3)] - m[idRw(1, 2, 3)] * m[idRw(2, 1, 3)];
	inverse[idRw(1, 0, 3)] = m[idRw(1, 2, 3)] * m[idRw(2, 0, 3)] - m[idRw(1, 0, 3)] * m[idRw(2, 2, 3)];
	inverse[idRw(2, 0, 3)] = m[idRw(1, 0, 3)] * m[idRw(2, 1, 3)] - m[idRw(1, 1, 3)] * m[idRw(2, 0, 3)];

	det = m[idRw(0, 0, 3)] * inverse[idRw(0, 0, 3)] + m[idRw(0, 1, 3)] * inverse[idRw(1, 0, 3)] + 
		  m[idRw(0, 2, 3)] * inverse[idRw(2, 0, 3)];

	if (fabs(det) < f_eps) {
		return;
	}

	invDet = 1.0f / det;

	inverse[idRw(0, 1, 3)] = m[idRw(0, 2, 3)] * m[idRw(2, 1, 3)] - m[idRw(0, 1, 3)] * m[idRw(2, 2, 3)];
	inverse[idRw(0, 2, 3)] = m[idRw(0, 1, 3)] * m[idRw(1, 2, 3)] - m[idRw(0, 2, 3)] * m[idRw(1, 1, 3)];
	inverse[idRw(1, 1, 3)] = m[idRw(0, 0, 3)] * m[idRw(2, 2, 3)] - m[idRw(0, 2, 3)] * m[idRw(2, 0, 3)];
	inverse[idRw(1, 2, 3)] = m[idRw(0, 2, 3)] * m[idRw(1, 0, 3)] - m[idRw(0, 0, 3)] * m[idRw(1, 2, 3)];
	inverse[idRw(2, 1, 3)] = m[idRw(0, 1, 3)] * m[idRw(2, 0, 3)] - m[idRw(0, 0, 3)] * m[idRw(2, 1, 3)];
	inverse[idRw(2, 2, 3)] = m[idRw(0, 0, 3)] * m[idRw(1, 1, 3)] - m[idRw(0, 1, 3)] * m[idRw(1, 0, 3)];

	rt[idRw(0, 0, 3)] = inverse[idRw(0, 0, 3)] * invDet;
	rt[idRw(0, 1, 3)] = inverse[idRw(0, 1, 3)] * invDet;
	rt[idRw(0, 2, 3)] = inverse[idRw(0, 2, 3)] * invDet;

	rt[idRw(1, 0, 3)] = inverse[idRw(1, 0, 3)] * invDet;
	rt[idRw(1, 1, 3)] = inverse[idRw(1, 1, 3)] * invDet;
	rt[idRw(1, 2, 3)] = inverse[idRw(1, 2, 3)] * invDet;

	rt[idRw(2, 0, 3)] = inverse[idRw(2, 0, 3)] * invDet;
	rt[idRw(2, 1, 3)] = inverse[idRw(2, 1, 3)] * invDet;
	rt[idRw(2, 2, 3)] = inverse[idRw(2, 2, 3)] * invDet;
}
