
#include <cmath>

#include "cmplx.h"

using namespace std;

cmplx cmplxSum(cmplx a, cmplx b) {
    return cmplx(a[0] + b[0], a[1] + b[1]);
}

cmplx cmplxSub(cmplx a, cmplx b) {
    return cmplx(a[0] - b[0], a[1] - b[1]);
}

cmplx cmplxMult(cmplx a, cmplx b) {
    return cmplx(a[0]*b[0] - a[1]*b[1], a[0]*b[1] + a[1]*b[0]);
}

cmplx cmplxDiv(cmplx a, cmplx b)  {
    cmplx rt;

    rt[0] = (a[0]*b[0] + a[1]*b[1]) / (b[0]*b[0] + b[1]*b[1]);
    rt[1] = (a[1]*b[0] - a[0]*b[1]) / (b[0]*b[0] + b[1]*b[1]);

    return rt;
}

cmplx cmplxScale(cmplx a, float k) {
    return cmplx(a[0]*k, a[1]*k);
}