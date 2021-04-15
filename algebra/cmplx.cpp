
#include <cmath>

#include "cmplx.h"

using namespace std;

cmplx_t cmplx_sum(cmplx_t a, cmplx_t b) {
    return cmplx_t(a[0] + b[0], a[1] + b[1]);
}

cmplx_t cmplx_sub(cmplx_t a, cmplx_t b) {
    return cmplx_t(a[0] - b[0], a[1] - b[1]);
}

cmplx_t cmplx_mult(cmplx_t a, cmplx_t b) {
    return cmplx_t(a[0]*b[0] - a[1]*b[1], a[0]*b[1] + a[1]*b[0]);
}

cmplx_t cmplx_div(cmplx_t a, cmplx_t b)  {
    cmplx_t rt;

    rt[0] = (a[0]*b[0] + a[1]*b[1]) / (b[0]*b[0] + b[1]*b[1]);
    rt[1] = (a[1]*b[0] - a[0]*b[1]) / (b[0]*b[0] + b[1]*b[1]);

    return rt;
}

cmplx_t cmplx_scale(cmplx_t a, float k) {
    return cmplx_t(a[0]*k, a[1]*k);
}