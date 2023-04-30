#ifndef __mesh_h_
#define __mesh_h_

#include <GL/gl.h>
#include <GL/glew.h>
#include <GL/glu.h>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include <glm/gtc/type_ptr.hpp>

#include "glm/glm.hpp"

struct SBodyStruct {
        std::vector<glm::vec3> bodyTriangles;
        std::vector<glm::vec3> bodyNormals;
        std::vector<glm::vec2> bodyTexCoords;

        float bodyYaw, bodyPitch, bodyRoll;
        float bodyYawVel, bodyPitchVel, bodyRollVel;

        glm::vec3 bodyPos;
        glm::vec3 bodyVel;

        glm::quat rotQuat;

        std::string mtrlName;
};

class CBodyBase {
    private:
        std::map<std::string, SBodyStruct> bodyList;

    public:
        CBodyBase();
        ~CBodyBase();

        CBodyBase(const CBodyBase& cbb) = delete;
        CBodyBase(const CBodyBase&& cbb) = delete;
        CBodyBase& operator=(const CBodyBase& cbb) = delete;
        CBodyBase& operator=(const CBodyBase&& cbb) = delete;

        void appendNewBody(const std::string& bName, const std::string& mtrlName);
        void appendNewBody(const std::string& bName,
                           const std::string& mtrlName,
                           const glm::vec3& scl);
        void setBodyParameters(const std::string& bName, const glm::vec3& offst);
        void setBodyParameters(const std::string& bName, float yaw, float pitch, float roll);
        void setBodyParameters(const std::string& bName,
                               float yaw,
                               float pitch,
                               float roll,
                               const glm::vec3& offst);

        void setRotQuat(const std::string& bName, const glm::quat& rot);

        void setBodyTranform(const std::string& bName, float yaw, float pitch, float roll);

        void updateBody(const std::string& bName, float dt);
        void renderBody(const std::string& bName);

        std::string getMtrlName(const std::string& bName);

        std::vector<std::string> getEntireBodyQueue();
};

#endif