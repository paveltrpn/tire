
#pragma once

#include <array>
#include <cmath>

#include "common.h"

class vec2 {
    public:
        float operator[](const int32_t id) const {
            return data[id];
        };

        float &operator[](const int32_t id) {
            return data[id];
        };

        vec2 &operator=(const vec2 &v) {
            data[0] = v[0];
            data[1] = v[1];

            return (*this);
        };

        vec2() : data{ 0.0, 0.0 } {};

        vec2(const float x, const float y) : data{ x, y } {};

        vec2(const vec2 &v) : data{ v[0], v[1] } {};

        ~vec2(){};

    private:
        std::array<float, 2> data;
};

float vec2Lenght(const vec2 &v);
vec2 vec2Normalize(const vec2 &v);
vec2 vec2Scale(const vec2 &v, const float scale);
vec2 vec2Invert(const vec2 &v);
float vec2Dot(const vec2 &a, const vec2 &b);
vec2 vec2Sum(const vec2 &a, const vec2 &b);
vec2 vec2Sub(const vec2 &a, const vec2 &b);
