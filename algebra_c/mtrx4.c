
#include <stdio.h>
#include <math.h>
#include "algebra_c.h"

void mtrx4Copy(const mtrx4 m, mtrx4 rt) {
    int32_t i, j;

	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			rt[idRw(i, j, 4)] = m[idRw(i, j, 4)];
		}
	}
}

void mtrx4Zero(mtrx4 m) {
    for (int i = 0; i < 16; i++) {
		m[i] = 0.0f;
	}
}

void mtrx4Set(float a00, float a01, float a02, float a03,
               float a10, float a11, float a12, float a13,
               float a20, float a21, float a22, float a23,
               float a30, float a31, float a32, float a33,
               mtrx4 rt) {
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
}

void mtrx4Idtt(mtrx4 rt) {
    int32_t i, j;

	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			if (i == j) {
				rt[idRw(i, j, 4)] = 1.0f;
			} else {
				rt[idRw(i, j, 4)] = 0.0f;
			}
		}
	}
}

void mtrx4LU(const mtrx4 m, mtrx4 lm, mtrx4 um) {
    int32_t	i, j, k; 
	float sum = 0;

	for (i = 0; i < 4; i++) {
		for (k = i; k < 4; k++) {
			sum = 0;
			for (j = 0; j < i; j++) {
				sum += (lm[idRw(i, j, 4)] * um[idRw(j, k, 4)]);
			}
			um[idRw(i, k, 4)] = m[idRw(i, k, 4)] - sum;
		}

		for (k = i; k < 4; k++) {
			if (i == k) {
				lm[idRw(i, i, 4)] = 1.0;
			} else {
				sum = 0;
				for (j = 0; j < i; j++) {
					sum += lm[idRw(k, j, 4)] * um[idRw(j, i, 4)];
				}
				lm[idRw(k, i, 4)] = (m[idRw(k, i, 4)] - sum) / um[idRw(i, i, 4)];
			}
		}
	}
}

void mtrx4LDLT(const mtrx4 m, mtrx4 lm, vec4 dv) {
    int32_t	i, j, k; 
	float sum;

	for (i = 0; i < 4; i++) {
		for (j = i; j < 4; j++) {
			sum = m[idRw(j, i, 4)];
			for (k = 0; k < i; k++) {
				sum = sum - lm[idRw(i, k, 4)]*dv[k]*lm[idRw(j, k, 4)];
				if (i == j) {
					if (sum <= 0) {
						printf("mtrx4LDLT(): matrix is not positive definite \n");
						mtrx3Idtt(lm);
						vec3Zero(dv);
						return;
					}
					dv[i] = sum;
					lm[idRw(i, i, 4)] = 1.0;
				} else {
					lm[idRw(j, i, 4)] = sum / dv[i];
				}
			}
		}
	}
}

void mtrx4GetTranspose(const mtrx4 m, mtrx4 rt) {
    int32_t i, j;
	float tmp;

	mtrx4Copy(m, rt);

	for (i = 0; i < 4; i++) {
		for (j = 0; j < i; j++) {
			tmp = rt[idRw(i, i, 4)];
			rt[idRw(i, j, 4)] = rt[idRw(j, i, 4)];
			rt[idRw(j, i, 4)] = tmp;
		}
	}
}

void mtrx4TranposeSelf(mtrx4 m) {
    int32_t i, j;
	float tmp;

	for (i = 0; i < 4; i++) {
		for (j = 0; j < i; j++) {
			tmp = m[idRw(i, i, 4)];
			m[idRw(i, j, 4)] = m[idRw(j, i, 4)];
			m[idRw(j, i, 4)] = tmp;
		}
	}
}

void mtrx4GetInv(const mtrx4 m, mtrx4 rt){
 	float det;
    int i;

    rt[0] = m[5]  * m[10] * m[15] - 
             m[5]  * m[11] * m[14] - 
             m[9]  * m[6]  * m[15] + 
             m[9]  * m[7]  * m[14] +
             m[13] * m[6]  * m[11] - 
             m[13] * m[7]  * m[10];

    rt[4] = -m[4]  * m[10] * m[15] + 
              m[4]  * m[11] * m[14] + 
              m[8]  * m[6]  * m[15] - 
              m[8]  * m[7]  * m[14] - 
              m[12] * m[6]  * m[11] + 
              m[12] * m[7]  * m[10];

    rt[8] = m[4]  * m[9] * m[15] - 
             m[4]  * m[11] * m[13] - 
             m[8]  * m[5] * m[15] + 
             m[8]  * m[7] * m[13] + 
             m[12] * m[5] * m[11] - 
             m[12] * m[7] * m[9];

    rt[12] = -m[4]  * m[9] * m[14] + 
               m[4]  * m[10] * m[13] +
               m[8]  * m[5] * m[14] - 
               m[8]  * m[6] * m[13] - 
               m[12] * m[5] * m[10] + 
               m[12] * m[6] * m[9];

    rt[1] = -m[1]  * m[10] * m[15] + 
              m[1]  * m[11] * m[14] + 
              m[9]  * m[2] * m[15] - 
              m[9]  * m[3] * m[14] - 
              m[13] * m[2] * m[11] + 
              m[13] * m[3] * m[10];

    rt[5] = m[0]  * m[10] * m[15] - 
             m[0]  * m[11] * m[14] - 
             m[8]  * m[2] * m[15] + 
             m[8]  * m[3] * m[14] + 
             m[12] * m[2] * m[11] - 
             m[12] * m[3] * m[10];

    rt[9] = -m[0]  * m[9] * m[15] + 
              m[0]  * m[11] * m[13] + 
              m[8]  * m[1] * m[15] - 
              m[8]  * m[3] * m[13] - 
              m[12] * m[1] * m[11] + 
              m[12] * m[3] * m[9];

    rt[13] = m[0]  * m[9] * m[14] - 
              m[0]  * m[10] * m[13] - 
              m[8]  * m[1] * m[14] + 
              m[8]  * m[2] * m[13] + 
              m[12] * m[1] * m[10] - 
              m[12] * m[2] * m[9];

    rt[2] = m[1]  * m[6] * m[15] - 
             m[1]  * m[7] * m[14] - 
             m[5]  * m[2] * m[15] + 
             m[5]  * m[3] * m[14] + 
             m[13] * m[2] * m[7] - 
             m[13] * m[3] * m[6];

    rt[6] = -m[0]  * m[6] * m[15] + 
              m[0]  * m[7] * m[14] + 
              m[4]  * m[2] * m[15] - 
              m[4]  * m[3] * m[14] - 
              m[12] * m[2] * m[7] + 
              m[12] * m[3] * m[6];

    rt[10] = m[0]  * m[5] * m[15] - 
              m[0]  * m[7] * m[13] - 
              m[4]  * m[1] * m[15] + 
              m[4]  * m[3] * m[13] + 
              m[12] * m[1] * m[7] - 
              m[12] * m[3] * m[5];

    rt[14] = -m[0]  * m[5] * m[14] + 
               m[0]  * m[6] * m[13] + 
               m[4]  * m[1] * m[14] - 
               m[4]  * m[2] * m[13] - 
               m[12] * m[1] * m[6] + 
               m[12] * m[2] * m[5];

    rt[3] = -m[1] * m[6] * m[11] + 
              m[1] * m[7] * m[10] + 
              m[5] * m[2] * m[11] - 
              m[5] * m[3] * m[10] - 
              m[9] * m[2] * m[7] + 
              m[9] * m[3] * m[6];

    rt[7] = m[0] * m[6] * m[11] - 
             m[0] * m[7] * m[10] - 
             m[4] * m[2] * m[11] + 
             m[4] * m[3] * m[10] + 
             m[8] * m[2] * m[7] - 
             m[8] * m[3] * m[6];

    rt[11] = -m[0] * m[5] * m[11] + 
               m[0] * m[7] * m[9] + 
               m[4] * m[1] * m[11] - 
               m[4] * m[3] * m[9] - 
               m[8] * m[1] * m[7] + 
               m[8] * m[3] * m[5];

    rt[15] = m[0] * m[5] * m[10] - 
              m[0] * m[6] * m[9] - 
              m[4] * m[1] * m[10] + 
              m[4] * m[2] * m[9] + 
              m[8] * m[1] * m[6] - 
              m[8] * m[2] * m[5];

    det = m[0] * rt[0] + m[1] * rt[4] + m[2] * rt[8] + m[3] * rt[12];

    if (det == 0) {
		printf("mtrx4GetInv(): determinant of inv matrix is zero\n");
		return;
	}
        

    det = 1.0 / det;

    for (i = 0; i < 16; i++)
        rt[i] = rt[i] * det;
}
