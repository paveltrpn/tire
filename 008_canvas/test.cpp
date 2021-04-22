
#include <iostream>
#include <utility>
#include <vector>
#include <cmath>
#include <array>

#include "canvas.h"

using namespace std;

/*	multidimensional array mapping, array[i][j]
	row-wise (C, C++):
	(0	1)
	(2	3)

	column-wise (Fortran, Matlab):
	(0	2)
	(1	3)
*/

#define M_PI 3.14159265358979323846

constexpr float f_eps = 5.96e-08;
constexpr float deg_to_rad(float deg) {
	return deg * M_PI/180.0f;
}

constexpr int32_t id_rw(int32_t i, int32_t j, int32_t n) {
	return (i*n + j);
};

constexpr int32_t id_cw(int32_t i, int32_t j, int32_t n) {
	return (j*n + i);
};

int main(int argc, char **argv) {
    float sin_i, cos_i;
    constexpr int32_t CANVAS = 512;
    constexpr int32_t CANVAS_ZERO = CANVAS / 2;
    constexpr int32_t CIRCLE_SIZE = 100;

    canvas_c anvas(CANVAS, CANVAS);

    anvas.set_pen_color(255, 0, 0);
    for (size_t j = 0; j < CANVAS; j++) {
        anvas.put_pixel(CANVAS / 2, j);
        anvas.put_pixel(j, CANVAS / 2);
    }

    anvas.set_pen_color(0, 0, 255);
    anvas.brasenham_circle({CANVAS / 2, CANVAS / 2}, 78);

    anvas.set_pen_color(0, 255, 0);
    for (size_t angl = 0; angl <= 180; angl = angl + 15) {
        sin_i = sin(deg_to_rad(angl));
        cos_i = cos(deg_to_rad(angl));

        anvas.brasenham_line({ int32_t(sin_i*CIRCLE_SIZE)+CANVAS_ZERO+93, int32_t(cos_i*CIRCLE_SIZE)+CANVAS_ZERO+93}, 
                             {-int32_t(sin_i*CIRCLE_SIZE)+CANVAS_ZERO+93,-int32_t(cos_i*CIRCLE_SIZE)+CANVAS_ZERO+93});
    }

    anvas.set_pen_color(255,100,10);
    for (size_t angl = 0; angl <= 180; angl = angl + 15) {
        sin_i = sin(deg_to_rad(angl));
        cos_i = cos(deg_to_rad(angl));

        anvas.wu_line({ int32_t(sin_i*CIRCLE_SIZE)+CANVAS_ZERO-101, int32_t(cos_i*CIRCLE_SIZE)+CANVAS_ZERO-101}, 
                      {-int32_t(sin_i*CIRCLE_SIZE)+CANVAS_ZERO-101,-int32_t(cos_i*CIRCLE_SIZE)+CANVAS_ZERO-101});
    }

    anvas.set_pen_color(100,10,255);
    for (size_t angl = 0; angl <= 180; angl = angl + 15) {
        sin_i = sin(deg_to_rad(angl));
        cos_i = cos(deg_to_rad(angl));

        anvas.dda_line({ int32_t(sin_i*CIRCLE_SIZE)+CANVAS_ZERO+45, int32_t(cos_i*CIRCLE_SIZE)+CANVAS_ZERO-114}, 
                       {-int32_t(sin_i*CIRCLE_SIZE)+CANVAS_ZERO+45,-int32_t(cos_i*CIRCLE_SIZE)+CANVAS_ZERO-114});
    }
    
    anvas.write_jpeg("out.jpeg");
    anvas.write_tga("out.tga");

    return 0;
}