
#pragma once

#include <array>
#include <cmath>
#include <tuple>

#include "common.h"
#include "qtnn.h"
#include "vec3.h"
#include "vec4.h"

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

        mtrx4 operator*(mtrx4 &arg);

        mtrx4()
            : data{ 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                    0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f } {};

        mtrx4(float arg) {
            for (auto &elem : data) {
                elem = arg;
            }
        };

        mtrx4(float a00,
              float a01,
              float a02,
              float a03,
              float a10,
              float a11,
              float a12,
              float a13,
              float a20,
              float a21,
              float a22,
              float a23,
              float a30,
              float a31,
              float a32,
              float a33)
            : data{
                  a00, a01, a02, a03, a10, a11, a12, a13, a20, a21, a22, a23, a30, a31, a32, a33
              } {};

        mtrx4(const mtrx4 &m)
            : data{ m[0], m[1], m[2],  m[3],  m[4],  m[5],  m[6],  m[7],
                    m[8], m[9], m[10], m[11], m[12], m[13], m[14], m[15] } {};

        ~mtrx4(){};

        float *getData() {
            return data.data();
        };

        void invertSelf() {
            mtrx4 inv;
            size_t i;
            float det;

            inv[0] = data[5] * data[10] * data[15] - data[5] * data[11] * data[14]
                     - data[9] * data[6] * data[15] + data[9] * data[7] * data[14]
                     + data[13] * data[6] * data[11] - data[13] * data[7] * data[10];

            inv[4] = -data[4] * data[10] * data[15] + data[4] * data[11] * data[14]
                     + data[8] * data[6] * data[15] - data[8] * data[7] * data[14]
                     - data[12] * data[6] * data[11] + data[12] * data[7] * data[10];

            inv[8] = data[4] * data[9] * data[15] - data[4] * data[11] * data[13]
                     - data[8] * data[5] * data[15] + data[8] * data[7] * data[13]
                     + data[12] * data[5] * data[11] - data[12] * data[7] * data[9];

            inv[12] = -data[4] * data[9] * data[14] + data[4] * data[10] * data[13]
                      + data[8] * data[5] * data[14] - data[8] * data[6] * data[13]
                      - data[12] * data[5] * data[10] + data[12] * data[6] * data[9];

            inv[1] = -data[1] * data[10] * data[15] + data[1] * data[11] * data[14]
                     + data[9] * data[2] * data[15] - data[9] * data[3] * data[14]
                     - data[13] * data[2] * data[11] + data[13] * data[3] * data[10];

            inv[5] = data[0] * data[10] * data[15] - data[0] * data[11] * data[14]
                     - data[8] * data[2] * data[15] + data[8] * data[3] * data[14]
                     + data[12] * data[2] * data[11] - data[12] * data[3] * data[10];

            inv[9] = -data[0] * data[9] * data[15] + data[0] * data[11] * data[13]
                     + data[8] * data[1] * data[15] - data[8] * data[3] * data[13]
                     - data[12] * data[1] * data[11] + data[12] * data[3] * data[9];

            inv[13] = data[0] * data[9] * data[14] - data[0] * data[10] * data[13]
                      - data[8] * data[1] * data[14] + data[8] * data[2] * data[13]
                      + data[12] * data[1] * data[10] - data[12] * data[2] * data[9];

            inv[2] = data[1] * data[6] * data[15] - data[1] * data[7] * data[14]
                     - data[5] * data[2] * data[15] + data[5] * data[3] * data[14]
                     + data[13] * data[2] * data[7] - data[13] * data[3] * data[6];

            inv[6] = -data[0] * data[6] * data[15] + data[0] * data[7] * data[14]
                     + data[4] * data[2] * data[15] - data[4] * data[3] * data[14]
                     - data[12] * data[2] * data[7] + data[12] * data[3] * data[6];

            inv[10] = data[0] * data[5] * data[15] - data[0] * data[7] * data[13]
                      - data[4] * data[1] * data[15] + data[4] * data[3] * data[13]
                      + data[12] * data[1] * data[7] - data[12] * data[3] * data[5];

            inv[14] = -data[0] * data[5] * data[14] + data[0] * data[6] * data[13]
                      + data[4] * data[1] * data[14] - data[4] * data[2] * data[13]
                      - data[12] * data[1] * data[6] + data[12] * data[2] * data[5];

            inv[3] = -data[1] * data[6] * data[11] + data[1] * data[7] * data[10]
                     + data[5] * data[2] * data[11] - data[5] * data[3] * data[10]
                     - data[9] * data[2] * data[7] + data[9] * data[3] * data[6];

            inv[7] = data[0] * data[6] * data[11] - data[0] * data[7] * data[10]
                     - data[4] * data[2] * data[11] + data[4] * data[3] * data[10]
                     + data[8] * data[2] * data[7] - data[8] * data[3] * data[6];

            inv[11] = -data[0] * data[5] * data[11] + data[0] * data[7] * data[9]
                      + data[4] * data[1] * data[11] - data[4] * data[3] * data[9]
                      - data[8] * data[1] * data[7] + data[8] * data[3] * data[5];

            inv[15] = data[0] * data[5] * data[10] - data[0] * data[6] * data[9]
                      - data[4] * data[1] * data[10] + data[4] * data[2] * data[9]
                      + data[8] * data[1] * data[6] - data[8] * data[2] * data[5];

            det = data[0] * inv[0] + data[1] * inv[4] + data[2] * inv[8] + data[3] * inv[12];

            if (det <= std::numeric_limits<float>::epsilon()) {
                return;
            }

            det = 1.0f / det;

            for (i = 0; i < 16; i++)
                data[i] = inv[i] * det;
        };

        void transposeSelf() {
            mtrx4 tmp;

            tmp[0] = data[0];
            tmp[1] = data[4];
            tmp[2] = data[8];
            tmp[3] = data[12];
            tmp[4] = data[1];
            tmp[5] = data[5];
            tmp[6] = data[9];
            tmp[7] = data[13];
            tmp[8] = data[2];
            tmp[9] = data[6];
            tmp[10] = data[10];
            tmp[11] = data[14];
            tmp[12] = data[3];
            tmp[13] = data[7];
            tmp[14] = data[11];
            tmp[15] = data[15];

            (*this) = tmp;
        };

    private:
        std::array<float, 16> data;
};

mtrx4 mtrx4FromIdtt();
mtrx4 mtrx4FromRtnYaw(float angl);
mtrx4 mtrx4FromRtnPitch(float angl);
mtrx4 mtrx4FromRtnRoll(float angl);
mtrx4 mtrx4FromEuler(float yaw, float pitch, float roll);
mtrx4 mtrx4FromAxisAngl(const vec3 &ax, float phi);
mtrx4 mtrx4FromOffset(const vec3 &src);
mtrx4 mtrx4FromScale(const vec3 &src);
mtrx4 mtrx4FromQtnn(const qtnn &src);
mtrx4 mtrx4FromPerspective(float fovy, float aspect, float near, float far);
mtrx4 mtrx4FromLookAt(vec3 eye, vec3 center, vec3 up);
mtrx4 mtrx4FromOrthographic(float left,
                            float right,
                            float bottom,
                            float top,
                            float near,
                            float far);

float mtrx4DetLU(const mtrx4 &m);
mtrx4 mtrx4Mult(const mtrx4 &a, const mtrx4 &b);
vec4 mtrx4MultVec(const mtrx4 &m, const vec3 &v);
vec3 mtrx4MultVec3(const mtrx4 &m, const vec3 &v);
tuple<mtrx4, mtrx4> mtrx4LU(const mtrx4 &m);
tuple<mtrx4, vec4> mtrx4LDLT(const mtrx4 &m);
mtrx4 mtrx4GetTranspose(const mtrx4 &m);
vec4 mtrx4SolveGauss(const mtrx4 &m, const vec4 &v);
mtrx4 mtrx4InsertCmn(const mtrx4 &m, const vec4 &v, int cmn);
vec4 mtrx4SolveKramer(const mtrx4 &m, const vec4 &v);
