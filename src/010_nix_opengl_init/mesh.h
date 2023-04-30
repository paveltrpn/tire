#ifndef __mesh_h_
#define __mesh_h_

#include <GL/gl.h>
#include <algebra2.h>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

using namespace std;

class box_c {
    public:
        box_c(){};
        ~box_c(){};

        void append(const vec3_t &pos, const mtrx3_t &spd);
        void show();

    private:
        /*  кортеж хранит:
            СМЕЩЕНИЕ (0)
            СКОРОСТЬ ПОВОРОТА (1)
            ПОЛОЖЕНИЕ (2) */
        vector<tuple<vec3_t, mtrx3_t, mtrx3_t>> orientation;
        const vec3_t base[8]
          = { { 1.0, 1.0, 1.0 },  { -1.0, 1.0, 1.0 },  { -1.0, -1.0, 1.0 },  { 1.0, -1.0, 1.0 },
              { 1.0, 1.0, -1.0 }, { -1.0, 1.0, -1.0 }, { -1.0, -1.0, -1.0 }, { 1.0, -1.0, -1.0 } };
        const vec3_t base_normal[6]
          = { { 0.0, 0.0, 1.0 },  { 0.0, 0.0, -1.0 }, { 0.0, 1.0, 0.0 },
              { 0.0, -1.0, 0.0 }, { 1.0, 0.0, 0.0 },  { -1.0, 0.0, 0.0 } };
        vec3_t clone[8];
        vec3_t clone_normal[6];
};

void draw_decart(float scale);

#endif