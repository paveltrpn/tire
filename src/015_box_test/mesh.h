#ifndef __mesh_h_
#define __mesh_h_

#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include <GL/gl.h>
#include <GL/glu.h>

#include "mtrx3.h"
#include "mtrx4.h"
#include "vec3.h"

struct SBodyStruct {
        std::vector<vec3> bodyTriangles;
        std::vector<vec3> bodyNormals;
        std::vector<vec2> bodyTexCoords;

        float bodyYaw, bodyPitch, bodyRoll;
        float bodyYawVel, bodyPitchVel, bodyRollVel;

        vec3 bodyPos;
        vec3 bodyVel;

        std::string mtrlName;
};

class CBodyBase {
    private:
        std::map<std::string, SBodyStruct> bodyList;

    public:
        CBodyBase();
        ~CBodyBase();

        void appendNewBody(const std::string& bName, const std::string& mtrlName);
        void appendNewBody(const std::string& bName, const std::string& mtrlName, const vec3& scl);
        void setBodyParameters(const std::string& bName, const vec3& offst);
        void setBodyParameters(const std::string& bName, float yaw, float pitch, float roll);
        void setBodyParameters(const std::string& bName,
                               float yaw,
                               float pitch,
                               float roll,
                               const vec3& offst);

        void setBodyTranform(const std::string& bName, float yaw, float pitch, float roll);

        void updateBody(const std::string& bName, float dt);
        void renderBody(const std::string& bName);

        std::string getMtrlName(const std::string& bName);

        std::vector<std::string> getEntireBodyQueue();
};

#endif
