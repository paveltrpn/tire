
#include <array>
#include <cmath>
#include <iostream>
#include <tuple>

#include "mtrx2.h"

constexpr size_t mrange = 2;

mtrx2 mtrx2Idtt() {
    mtrx2 rt;

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

mtrx2 mtrx2Set(float m[4]) {
    mtrx2 rt;

    for (size_t i = 0; i < mrange; i++) {
        for (size_t j = 0; j < mrange; j++) {
            rt[idRw(i, j, mrange)] = m[idRw(i, j, mrange)];
        }
    }

    return rt;
}

mtrx2 mtrx2SetFloat(float a00, float a01, float a10, float a11) {
    mtrx2 rt;

    rt[0] = a00;
    rt[1] = a01;
    rt[2] = a10;
    rt[3] = a11;

    return rt;
}

mtrx2 mtrx2Rtn(float phi) {
    mtrx2 rt;

    float cosphi, sinphi;

    sinphi = sinf(phi);
    cosphi = cosf(phi);

    rt[0] = cosphi;
    rt[1] = -sinphi;
    rt[2] = -sinphi;
    rt[3] = cosphi;

    return rt;
}

float mtrx2Det(mtrx2 m) {
    return m[0] * m[3] - m[1] * m[2];
}

float mtrx2_det_lu(mtrx2 m) {
    mtrx2 l, u;
    std::tuple<mtrx2, mtrx2> lu;
    float l_det, u_det;

    lu = mtrx2LU(m);

    l_det = std::get<0>(lu)[0];
    u_det = std::get<1>(lu)[0];

    for (size_t i = 1; i < mrange; i++) {
        l_det *= l[idRw(i, i, mrange)];
        u_det *= u[idRw(i, i, mrange)];
    }

    return l_det * u_det;
}

mtrx2 mtrx2Mult(mtrx2 a, mtrx2 b) {
    mtrx2 rt = { 0.0f };

    for (size_t i = 0; i < mrange; i++) {
        for (size_t j = 0; j < mrange; j++) {
            for (size_t k = 0; k < mrange; k++) {
                rt[idRw(i, j, mrange)] += a[idRw(k, j, mrange)] * b[idRw(i, k, mrange)];
            }
        }
    }

    return rt;
}

vec2 mtrx2MultVec(mtrx2 m, vec2 v) {
    vec2 rt;

    for (size_t i = 0; i < mrange; i++) {
        for (size_t j = 0; j < mrange; j++) {
            rt[i] += m[idRw(i, j, mrange)] * v[j];
        }
    }

    return rt;
}

/*
    Нижнетреугольная (L, lm) матрица имеет единицы по диагонали
*/

std::tuple<mtrx2, mtrx2> mtrx2LU(mtrx2 m) {
    mtrx2 lm, um;
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

    return { lm, um };
}

std::tuple<mtrx2, vec2> mtrx2LDLT(mtrx2 m) {
    mtrx2 lm;
    vec2 dv;
    size_t i, j, k;
    float sum;

    for (i = 0; i < mrange; i++) {
        for (j = i; j < mrange; j++) {
            sum = m[idRw(j, i, mrange)];
            for (k = 0; k < i; k++) {
                sum = sum - lm[idRw(i, k, mrange)] * dv[k] * lm[idRw(j, k, mrange)];
                if (i == j) {
                    if (sum <= 0) {
                        std::cout << "mtrx2_ldlt(): matrix is not positive deﬁnite\n";
                        return { mtrx2Idtt(), vec2(0.0, 0.0) };
                    }
                    dv[i] = sum;
                    lm[idRw(i, i, mrange)] = 1.0;
                } else {
                    lm[idRw(j, i, mrange)] = sum / dv[i];
                }
            }
        }
    }

    return { lm, dv };
}

mtrx2 mtrx2Transpose(mtrx2 m) {
    float tmp;
    mtrx2 rt;

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

mtrx2 mtrx2Invert(mtrx2 m) {
    mtrx2 rt;
    float det;

    det = mtrx2Det(m);

    if (fabs(det) < f_eps) {
        std::cout << "mtrx2_invert(): determinant is a zero!\n";
        return mtrx2Idtt();
    }

    return mtrx2(m[3], -m[1] / det, -m[2] / det, m[0] / det);
}

vec2 mtrx2SolveGauss(mtrx2 m, vec2 v) {
    size_t i, j, k;
    // float a[mrange][mrange + 1];
    float t;
    std::array<std::array<float, mrange + 1>, mrange> a;
    vec2 rt;

    for (i = 0; i < mrange; i++) {      //было ++i
        for (j = 0; j < mrange; j++) {  //было ++j
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
            t = a[j][k - 1] / a[k - 1][k - 1];
            for (i = 0; i < mrange + 1; i++) {
                a[j][i] = a[j][i] - t * a[k - 1][i];
            }
        }
    }

    /* обратный ход */
    for (i = mrange - 1; i > 0; i--) {
        rt[i] = a[i][mrange] / a[i][i];
        for (j = mrange - 1; j > i; j--) {
            rt[i] = rt[i] - a[i][j] * rt[j] / a[i][i];
        }
    }

    return rt;
}

mtrx2 mtrx2InsertCmn(mtrx2 m, vec2 v, int cmn) {
    size_t i, j = 0;
    mtrx2 rt;

    rt = m;

    for (i = cmn; i < mrange * mrange; i += mrange) {
        rt[i] = v[j];
        j++;
    }

    return rt;
}

vec2 mtrx2SolveKramer(mtrx2 m, vec2 v) {
    float det;
    mtrx2 kr_mtrx;
    vec2 rt;

    det = mtrx2Det(m);

    if (fabs(det) < f_eps) {
        std::cout << "mtrx2_solve_kramer(): system has no solve\n";
        return vec2(0.0, 0.0);
    }

    for (size_t i = 0; i < mrange; i++) {
        kr_mtrx = mtrx2InsertCmn(m, v, i);
        rt[i] = mtrx2Det(kr_mtrx) / det;
    }

    return rt;
}
