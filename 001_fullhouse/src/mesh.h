#ifndef __mesh_h_
#define __mesh_h_

#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <tuple>
	
#include <GL/gl.h>

#include "algebra2.h"

using namespace std;

class box_c {
    public:
        box_c() {};
        ~box_c() {};

        void append(const vec3 &pos, const mtrx3 &spd);
        void show();

    private:
        /*  кортеж хранит: 
            СМЕЩЕНИЕ (0) 
            СКОРОСТЬ ПОВОРОТА (1)
            ПОЛОЖЕНИЕ (2) */
        vector<tuple<vec3, mtrx3, mtrx3>> orientation;
        const vec3 base[8] = {{ 1.0, 1.0, 1.0},
	                            {-1.0, 1.0, 1.0},
	                            {-1.0,-1.0, 1.0},
	                            { 1.0,-1.0, 1.0},
	                            { 1.0, 1.0,-1.0},
	                            {-1.0, 1.0,-1.0},
	                            {-1.0,-1.0,-1.0},
	                            { 1.0,-1.0,-1.0}};
        const vec3 base_normal[6] = {{ 0.0, 0.0, 1.0},
	                                   { 0.0, 0.0,-1.0},
	                                   { 0.0, 1.0, 0.0},
	                                   { 0.0,-1.0, 0.0},
	                                   { 1.0, 0.0, 0.0},
	                                   {-1.0, 0.0, 0.0}}; 
        vec3 clone[8];
        vec3 clone_normal[6];
};

void draw_decart(float scale);

#endif