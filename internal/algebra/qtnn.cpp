
#include <cmath>
#include <iostream>

#include "qtnn.h"

qtnn::qtnn(float yaw, float pitch, float roll) {
    qtnn qyaw, qpitch, qroll, rt;
    vec3 vyaw, vpitch, vroll;

    vyaw = vec3(1.0, 0.0, 0.0);
    vpitch = vec3(0.0, 1.0, 0.0);
    vroll = vec3(0.0, 0.0, 1.0);

    qyaw = qtnn(vyaw, yaw);
    qpitch = qtnn(vpitch, pitch);
    qroll = qtnn(vroll, roll);

    rt = qtnnMult(qyaw, qpitch);

    rt = qtnnMult(rt, qroll);

    data[3] = rt[3];
    data[0] = rt[0];
    data[1] = rt[1];
    data[2] = rt[2];
}

qtnn qtnnScale(const qtnn &q, float scale) {
    qtnn rt;

    rt[3] = q[3] * scale;
    rt[0] = q[0] * scale;
    rt[1] = q[1] * scale;
    rt[2] = q[2] * scale;

    return rt;
}

qtnn qtnnSum(const qtnn &a, const qtnn &b) {
    qtnn rt;

    rt[0] = a[0] + b[0];
    rt[1] = a[1] + b[1];
    rt[2] = a[2] + b[2];
    rt[3] = a[3] + b[3];

    return rt;
}

qtnn qtnnSub(const qtnn &a, const qtnn &b) {
    qtnn rt;

    rt[0] = a[0] - b[0];
    rt[1] = a[1] - b[1];
    rt[2] = a[2] - b[2];
    rt[3] = a[3] - b[3];

    return rt;
}

float qtnnDot(const qtnn &a, const qtnn &b) {
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3];
}

qtnn qtnnMult(const qtnn &a, const qtnn &b) {
    qtnn rt;

    rt[3] = a[3] * b[3] - a[0] * b[0] - a[1] * b[1] - a[2] * b[2];
    rt[0] = a[3] * b[0] + a[0] * b[3] + a[1] * b[2] - a[2] * b[1];
    rt[1] = a[3] * b[1] - a[0] * b[2] + a[1] * b[3] + a[2] * b[0];
    rt[2] = a[3] * b[2] + a[0] * b[1] - a[1] * b[0] + a[2] * b[3];

    return rt;
}

qtnn qtnnSlerp(qtnn from, qtnn to, float t) {
    qtnn rt;
    float p1[4];
    float omega, cosom, sinom, scale0, scale1;

    // косинус угла
    cosom = qtnnDot(from, to);

    if (cosom < 0.0) {
        cosom = -cosom;
        p1[0] = -to[0];
        p1[1] = -to[1];
        p1[2] = -to[2];
        p1[3] = -to[3];
    } else {
        p1[0] = to[0];
        p1[1] = to[1];
        p1[2] = to[2];
        p1[3] = to[3];
    }

    if ((1.0 - cosom) > f_eps) {
        // стандартный случай (slerp)
        omega = acos(cosom);
        sinom = sinf(omega);
        scale0 = sinf((1.0 - t) * omega) / sinom;
        scale1 = sinf(t * omega) / sinom;
    } else {
        // если маленький угол - линейная интерполяция
        scale0 = 1.0 - t;
        scale1 = t;
    }

    rt[0] = scale0 * from[0] + scale1 * p1[0];
    rt[1] = scale0 * from[1] + scale1 * p1[1];
    rt[2] = scale0 * from[2] + scale1 * p1[2];
    rt[3] = scale0 * from[3] + scale1 * p1[3];

    return rt;
}
