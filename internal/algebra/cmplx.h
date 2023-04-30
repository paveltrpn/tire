
#pragma once

#include <array>
#include <cmath>

#include "common.h"

using namespace std;

class cmplx {
    public:
        float operator[](const int32_t id) const {
            return data[id];
        };

        float &operator[](const int32_t id) {
            return data[id];
        };

        cmplx &operator=(const cmplx &v) {
            data[_XC] = v[_XC];
            data[_YC] = v[_YC];

            return (*this);
        };

        cmplx() : data{ 0.0, 0.0 } {};

        cmplx(const float x, const float y) : data{ x, y } {};

        cmplx(const cmplx &v) : data{ v[_XC], v[_YC] } {};

        ~cmplx(){};

    private:
        std::array<float, 2> data;
};

cmplx cmplxSum(const cmplx &a, const cmplx &b);
cmplx cmplxSub(const cmplx &a, const cmplx &b);
cmplx cmplxMult(const cmplx &a, const cmplx &b);
cmplx cmplxDiv(const cmplx &a, const cmplx &b);
cmplx cmplxScale(const cmplx &a, float k);
