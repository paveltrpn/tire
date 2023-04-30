
#include <cmath>
#include <iostream>

#include "vec3.h"

using namespace std;

vec3 vec3Scale(const vec3 &v, float scale) {
    vec3 rt;

    rt[0] = v[0] * scale;
    rt[1] = v[1] * scale;
    rt[2] = v[2] * scale;

    return rt;
}

vec3 vec3Invert(const vec3 &v) {
    vec3 rt;

    rt[0] = -v[0];
    rt[1] = -v[1];
    rt[2] = -v[2];

    return rt;
}

float vec3Dot(const vec3 &a, const vec3 &b) {
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

vec3 vec3Sum(const vec3 &a, const vec3 &b) {
    vec3 rt;

    rt[0] = a[0] + b[0];
    rt[1] = a[1] + b[1];
    rt[2] = a[2] + b[2];

    return rt;
}

vec3 vec3Sub(const vec3 &a, const vec3 &b) {
    vec3 rt;

    rt[0] = a[0] - b[0];
    rt[1] = a[1] - b[1];
    rt[2] = a[2] - b[2];

    return rt;
}

vec3 vec3Cross(const vec3 &a, const vec3 &b) {
    vec3 rt;

    rt[0] = a[1] * b[2] - a[2] * b[1];
    rt[1] = a[2] * b[0] - a[0] * b[2];
    rt[2] = a[0] * b[1] - a[1] * b[0];

    return rt;
}