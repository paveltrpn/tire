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
        BasicBody(int type);
        BasicBody(int type, vec3 scl);

        ~BasicBody() {};

        void setOrientation(float yaw, float pitch, float roll);
        void setOffset(vec3 offst);

        void bodyMove(vec3 offst);
        void bodyRotate(float yaw, float pitch, float roll);

        void updateAndDraw();

    private:
        std::vector<vec3> BodyTriangles;
        std::vector<vec3> BodyNormals;

        float m_bodyYaw, m_bodyPitch, m_bodyRoll;

        vec3 m_bodyOffset;
};

#endif