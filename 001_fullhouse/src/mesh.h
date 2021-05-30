#ifndef __mesh_h_
#define __mesh_h_

#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <tuple>
	
#include <GL/gl.h>

#include "vec3.h"
#include "mtrx3.h"
#include "mtrx4.h"

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

class Body {
    public:
        enum {BOX, PRISM, ICOSAHEDRON};

        Body(int type, vec3 offst, vec3 orn);

        ~Body();

        // Body(const Body& in) = delete;
        // Body& operator=(const Body& in) = delete;
        // Body(Body&& in) = delete;
        // Body&& operator=(Body&& in) = delete;

        void setOrientation(float yaw, float pitch, float roll);
        void setOffset(float dx, float dy, float dz);

        void updateAndDraw();

    private:
        std::vector<vec3> BodyTriangles;
        std::vector<vec3> BodyNormals;

        vec3 BodyOrientation;
        vec3 BodyOffset;
        vec3 bodyScale;
};

void draw_decart(float scale);

#endif