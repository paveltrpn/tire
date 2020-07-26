
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

constexpr int32_t id_rw(int32_t i, int32_t j, int32_t n) {
	return (i*n + j);
};

constexpr int32_t id_cw(int32_t i, int32_t j, int32_t n) {
	return (j*n + i);
};

int main(int argc, char **argv) {
    canvas_c anvas(256, 256);

    anvas.set_pen_color(255, 0, 0);
    for (size_t j = 0; j < 256; j++) {
        anvas.put_pixel(128, j);
        anvas.put_pixel(j, 128);
    }
    
    anvas.set_pen_color(0, 255, 0);
    anvas.brasenham_line({0,0}, {255, 255});
    anvas.brasenham_line({200,190}, {130, 190});
    anvas.brasenham_line({71,190}, {10, 200});
    anvas.brasenham_line({120,50}, {200, 10});
    anvas.brasenham_line({0,255}, {255, 0});

    anvas.set_pen_color(0, 0, 255);
    anvas.brasenham_circle({128, 128}, 50);

    anvas.set_pen_color(255,255,255);
    anvas.wu_line({10,56}, {240,200});
    
    anvas.write_jpeg("out.jpeg");

    return 0;
}