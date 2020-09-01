
#include <iostream>
#include <cmath>

#include "qtnn.h"

void qtnn_show(const qtnn_t &q) {
	printf("%5.2f %5.2f %5.2f %5.2f\n", q[_XC], q[_YC], q[_ZC], q[_WC]);
}

float qtnn_lenght(const qtnn_t &q) {
	return sqrtf(q[_XC]*q[_XC] +
				 q[_YC]*q[_YC] +
				 q[_ZC]*q[_ZC] +
				 q[_WC]*q[_WC]);
}

qtnn_t qtnn_normalize(const qtnn_t &q) {
	qtnn_t rt;
	float len;

	len = qtnn_lenght(q);

	if (len > f_eps) {
		rt[_WC] = q[_WC] / len;
		rt[_XC] = q[_XC] / len;
		rt[_YC] = q[_YC] / len;
		rt[_ZC] = q[_ZC] / len;
	} else {
		printf("qtnn_normalize(): quaternion is too short!");
		return qtnn_t();
	}

	return rt;
}

qtnn_t qtnn_invert(const qtnn_t &q) {
	qtnn_t rt;

	rt[_WC] =  q[_WC];
	rt[_XC] = -q[_XC];
	rt[_YC] = -q[_YC];
	rt[_ZC] = -q[_ZC];

	return rt;
}

qtnn_t qtnn_scale(const qtnn_t &q, float scale) {
	qtnn_t rt;

	rt[_WC] = q[_WC] * scale;
	rt[_XC] = q[_XC] * scale;
	rt[_YC] = q[_YC] * scale;
	rt[_ZC] = q[_ZC] * scale;

	return rt;
}

qtnn_t qtnn_sum(const qtnn_t &a, const qtnn_t &b) {
	qtnn_t rt;

	rt[0] = a[0] + b[0];
	rt[1] = a[1] + b[1];
	rt[2] = a[2] + b[2];
	rt[3] = a[3] + b[3];

	return rt;
}

qtnn_t qtnn_sub(const qtnn_t &a, const qtnn_t &b) {
	qtnn_t rt;

	rt[0] = a[0] - b[0];
	rt[1] = a[1] - b[1];
	rt[2] = a[2] - b[2];
	rt[3] = a[3] - b[3];

	return rt;
}

float qtnn_dot(const qtnn_t &a, const qtnn_t &b) {
	return a[0]*b[0] + a[1]*b[1] + a[2]*b[2] + a[3]*b[3];
}

qtnn_t qtnn_mult(const qtnn_t &a, const qtnn_t &b) {
	qtnn_t rt;

	rt[_WC] = a[_WC]*b[_WC] - a[_XC]*b[_XC] - a[_YC]*b[_YC] - a[_ZC]*b[_ZC];
	rt[_XC] = a[_WC]*b[_XC] + a[_XC]*b[_WC] + a[_YC]*b[_ZC] - a[_ZC]*b[_YC];
	rt[_YC] = a[_WC]*b[_YC] - a[_XC]*b[_ZC] + a[_YC]*b[_WC] + a[_ZC]*b[_XC];
	rt[_ZC] = a[_WC]*b[_ZC] + a[_XC]*b[_YC] - a[_YC]*b[_XC] + a[_ZC]*b[_WC];

	return rt;
}

/* function is broken */
qtnn_t qtnn_mult_vec3(const qtnn_t &a, const vec3_t &b) {
	qtnn_t rt;

	rt[_WC] = -a[_WC]*b[_XC] - a[_YC]*b[_YC] - a[_ZC]*b[_ZC];
	rt[_XC] =  a[_WC]*b[_XC] + a[_YC]*b[_ZC] - a[_ZC]*b[_YC];
	rt[_YC] =  a[_WC]*b[_YC] - a[_XC]*b[_ZC] + a[_ZC]*b[_XC];
	rt[_ZC] =  a[_WC]*b[_ZC] + a[_XC]*b[_YC] - a[_YC]*b[_XC];

	return rt;
}

qtnn_t::qtnn_t(const vec3_t &ax, float phi) {
    float sinhalfphi;

	sinhalfphi = sinf(deg_to_rad(phi * 0.5f));

	data[_WC] = cosf(deg_to_rad(phi * 0.5f));
	data[_XC] = ax[_XC] * sinhalfphi;
	data[_YC] = ax[_YC] * sinhalfphi;
	data[_ZC] = ax[_ZC] * sinhalfphi;

}

qtnn_t::qtnn_t(float yaw, float pitch, float roll) {
    qtnn_t	qyaw, qpitch, qroll, rt;
	vec3_t  vyaw, vpitch, vroll;

	vyaw   = vec3_t(1.0, 0.0, 0.0);
	vpitch = vec3_t(0.0, 1.0, 0.0);
	vroll  = vec3_t(0.0, 0.0, 1.0);

	qyaw   = qtnn_t(vyaw, yaw);
	qpitch = qtnn_t(vpitch, pitch);
	qroll  = qtnn_t(vroll, roll);

	rt = qtnn_mult(qyaw, qpitch);

	rt = qtnn_mult(rt, qroll);

	data[_WC] = rt[_WC]; 
	data[_XC] = rt[_XC];
	data[_YC] = rt[_YC];
	data[_ZC] = rt[_ZC];
}

vec3_t qtnn_to_vec3(const qtnn_t &q) {
	return vec3_t(q[_XC], q[_YC], q[_ZC]);;
}

vec3_t qtnn_transform_vec3(const qtnn_t &a, const vec3_t &b) {
	vec3_t rt;
	qtnn_t	vq, tmp, ia;

	vq = qtnn_t(b);
    tmp = qtnn_mult(a, vq);
	ia = qtnn_invert(a);
	rt = qtnn_to_vec3(qtnn_mult(tmp, ia));

	return rt;
}
