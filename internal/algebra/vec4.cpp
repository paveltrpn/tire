
#include <cmath>
#include <iostream>

#include "vec4.h"

vec4 vec4Copy(vec4 &v) {
    vec4 rt;

    rt[0] = v[0];
    rt[1] = v[1];
    rt[2] = v[2];
    rt[3] = v[3];

    return rt;
}

vec4 vec4Set(float x, float y, float z, float w) {
    vec4 rt;

    rt[0] = x;
    rt[1] = y;
    rt[2] = z;
    rt[3] = w;

    return rt;
}

float vec4Lenght(vec4 &v) {
    return sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2] + v[3] * v[3]);
}

vec4 vec4Normalize(vec4 &v) {
    vec4 rt;
    float len;

    len = vec4Lenght(v);

    if (len != 0.0f) {
        rt[0] = v[0] / len;
        rt[1] = v[1] / len;
        rt[2] = v[2] / len;
        rt[3] = v[3] / len;
    }

    return rt;
}

vec4 vec4Scale(vec4 &v, float scale) {
    vec4 rt;

    rt[0] = v[0] * scale;
    rt[1] = v[1] * scale;
    rt[2] = v[2] * scale;
    rt[3] = v[3] * scale;

    return rt;
}

vec4 vec4Invert(vec4 &v) {
    vec4 rt;

    rt[0] = -v[0];
    rt[1] = -v[1];
    rt[2] = -v[2];
    rt[3] = -v[3];

    return rt;
}

float vec4Dot(vec4 &a, vec4 &b) {
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3];
}

vec4 vec4Sum(vec4 a, vec4 b) {
    vec4 rt;

    rt[0] = a[0] + b[0];
    rt[1] = a[1] + b[1];
    rt[2] = a[2] + b[2];
    rt[3] = a[3] + b[3];

    return rt;
}

vec4 vec4Sub(vec4 &a, vec4 &b) {
    vec4 rt;

    rt[0] = a[0] - b[0];
    rt[1] = a[1] - b[1];
    rt[2] = a[2] - b[2];
    rt[3] = a[3] - b[3];

    return rt;
}
