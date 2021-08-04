#ifndef __mesh_h_
#define __mesh_h_

#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <map>

#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/gl.h>

#include "glm/glm.hpp"
#include <glm/gtx/transform.hpp>

struct SBodyStruct {
    std::vector<glm::vec3> bodyTriangles;
    std::vector<glm::vec3> bodyNormals;
    std::vector<glm::vec2> bodyTexCoords;

    float bodyYaw, bodyPitch, bodyRoll;
    float bodyYawVel, bodyPitchVel, bodyRollVel;

    glm::vec3 bodyPos;
    glm::vec3 bodyVel;

    std::string mtrlName;
};

class CBodyBase {
    private:
        std::map<std::string, SBodyStruct> bodyList;

    public:
        CBodyBase();
        ~CBodyBase();

        void appendNewBody(const std::string& bName, const std::string& mtrlName);
        void appendNewBody(const std::string& bName, const std::string& mtrlName, const glm::vec3& scl);
        void setBodyParameters(const std::string& bName, const glm::vec3& offst);
        void setBodyParameters(const std::string& bName, float yaw, float pitch, float roll);
        void setBodyParameters(const std::string& bName, float yaw, float pitch, float roll, const glm::vec3& offst);

        void setBodyTranform(const std::string& bName, float yaw, float pitch, float roll);

        void updateBody(const std::string& bName, float dt);
        void renderBody(const std::string& bName);

        std::string getMtrlName(const std::string& bName);

        std::vector<std::string> getEntireBodyQueue();
};

#endif