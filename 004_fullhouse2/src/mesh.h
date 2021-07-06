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

// 
// BasicBody need to be drawn by itself ?????
// 
class BasicBody {
    public:
        enum {BOX, PRISM, ICOSAHEDRON};

        BasicBody() {};
        BasicBody(int type, vec3 offst, vec3 orn, vec3 scl);

        ~BasicBody();

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
        vec3 BodyScale;
};

#endif