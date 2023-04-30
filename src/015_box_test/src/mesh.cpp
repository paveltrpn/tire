
#include <array>
#include <tuple>

#include "apputils.h"
#include "mesh.h"
#include "qtnn.h"

// По три нормали на вершину!!!
std::vector<vec3> boxTris = {
    { 1.0f, 1.0f, 1.0f },   { -1.0f, 1.0f, 1.0f },  { -1.0f, -1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f },
    { -1.0f, -1.0f, 1.0f }, { 1.0f, -1.0f, 1.0f },  { 1.0f, -1.0f, -1.0f }, { 1.0f, -1.0f, 1.0f },
    { -1.0f, -1.0f, 1.0f }, { 1.0f, -1.0f, -1.0f }, { -1.0f, -1.0f, 1.0f }, { -1.0f, -1.0f, -1.0f },
    { -1.0f, -1.0, -1.0 },  { -1.0f, -1.0, 1.0 },   { -1.0f, 1.0, 1.0 },    { -1.0f, -1.0f, -1.0 },
    { -1.0f, 1.0f, 1.0 },   { -1.0f, 1.0f, -1.0 },  { -1.0f, 1.0f, -1.0f }, { 1.0f, 1.0f, -1.0f },
    { 1.0f, -1.0f, -1.0f }, { -1.0f, 1.0f, -1.0 },  { 1.0f, -1.0f, -1.0 },  { -1.0f, -1.0f, -1.0 },
    { 1.0f, 1.0f, -1.0 },   { 1.0f, 1.0f, 1.0 },    { 1.0f, -1.0f, 1.0 },   { 1.0f, 1.0f, -1.0 },
    { 1.0f, -1.0f, 1.0 },   { 1.0f, -1.0f, -1.0 },  { -1.0f, 1.0f, -1.0f }, { -1.0f, 1.0f, 1.0f },
    { 1.0f, 1.0f, 1.0f },   { -1.0f, 1.0, -1.0f },  { 1.0f, 1.0, 1.0f },    { 1.0f, 1.0, -1.0f }
};

std::vector<vec3> boxNrmls
  = { { 0.0f, 0.0f, 1.0f },  { 0.0f, 0.0f, 1.0f },  { 0.0f, 0.0f, 1.0f },  { 0.0f, 0.0f, 1.0f },
      { 0.0f, 0.0f, 1.0f },  { 0.0f, 0.0f, 1.0f },  { 0.0f, -1.0f, 0.0f }, { 0.0f, -1.0f, 0.0f },
      { 0.0f, -1.0f, 0.0f }, { 0.0f, -1.0f, 0.0f }, { 0.0f, -1.0f, 0.0f }, { 0.0f, -1.0f, 0.0f },
      { -1.0f, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f },
      { -1.0f, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f, -1.0f },
      { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f, -1.0f },
      { 1.0f, 0.0f, 0.0f },  { 1.0f, 0.0f, 0.0f },  { 1.0f, 0.0f, 0.0f },  { 1.0f, 0.0f, 0.0f },
      { 1.0f, 0.0f, 0.0f },  { 1.0f, 0.0f, 0.0f },  { 0.0f, 1.0f, 0.0f },  { 0.0f, 1.0f, 0.0f },
      { 0.0f, 1.0f, 0.0f },  { 0.0f, 1.0f, 0.0f },  { 0.0f, 1.0f, 0.0f },  { 0.0f, 1.0f, 0.0f } };

std::vector<vec2> boxTexCoords = {
    { 0.0f, 0.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 0.0f, 0.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f },
    { 0.0f, 0.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 0.0f, 0.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f },
    { 0.0f, 0.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 0.0f, 0.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f },
    { 0.0f, 0.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 0.0f, 0.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f },
    { 0.0f, 0.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 0.0f, 0.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f },
    { 0.0f, 0.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 0.0f, 0.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f }
};

CBodyBase::CBodyBase() {
}

CBodyBase::~CBodyBase() {
}

void CBodyBase::appendNewBody(const std::string& bName, const std::string& mtrlName) {
    SBodyStruct tmp;

    tmp.bodyYaw = tmp.bodyPitch = tmp.bodyRoll = 0.0f;
    tmp.bodyYawVel = tmp.bodyPitchVel = tmp.bodyRollVel = 0.0f;
    tmp.bodyPos = vec3();

    tmp.bodyTriangles = boxTris;
    tmp.bodyNormals = boxNrmls;
    tmp.bodyTexCoords = boxTexCoords;

    tmp.mtrlName = mtrlName;

    bodyList.insert({ bName, tmp });
}

void CBodyBase::appendNewBody(const std::string& bName,
                              const std::string& mtrlName,
                              const vec3& scl) {
    SBodyStruct tmp;

    tmp.bodyYaw = tmp.bodyPitch = tmp.bodyRoll = 0.0f;
    tmp.bodyYawVel = tmp.bodyPitchVel = tmp.bodyRollVel = 0.0f;
    tmp.bodyPos = vec3();

    tmp.bodyTriangles = boxTris;
    tmp.bodyNormals = boxNrmls;
    tmp.bodyTexCoords = boxTexCoords;

    tmp.mtrlName = mtrlName;

    mtrx4 scaleMtrx = mtrx4FromScale(scl);
    for (auto& vert : tmp.bodyTriangles) {
        vert = mtrx4MultVec3(scaleMtrx, vert);
    }

    bodyList.insert({ bName, tmp });
}

void CBodyBase::setBodyParameters(const std::string& bName, const vec3& offst) {
    auto bdy = bodyList.find(bName);

    if (bdy != bodyList.end()) {
        bdy->second.bodyPos = offst;
    } else {
        std::cout << "CBodyBase::setBodyParameters(): ERROR! Can't find body - " << bName
                  << std::endl;
    }
}

void CBodyBase::setBodyParameters(const std::string& bName, float yaw, float pitch, float roll) {
    auto bdy = bodyList.find(bName);

    if (bdy != bodyList.end()) {
        bdy->second.bodyYaw = yaw;
        bdy->second.bodyPitch = pitch;
        bdy->second.bodyRoll = roll;
    } else {
        std::cout << "CBodyBase::setBodyParameters(): ERROR! Can't find body - " << bName
                  << std::endl;
    }
}

void CBodyBase::setBodyParameters(const std::string& bName,
                                  float yaw,
                                  float pitch,
                                  float roll,
                                  const vec3& offst) {
    auto bdy = bodyList.find(bName);

    if (bdy != bodyList.end()) {
        bdy->second.bodyPos = offst;
        bdy->second.bodyYaw = yaw;
        bdy->second.bodyPitch = pitch;
        bdy->second.bodyRoll = roll;
    } else {
        std::cout << "CBodyBase::setBodyParameters(): ERROR! Can't find body - " << bName
                  << std::endl;
    }
}

void CBodyBase::setBodyTranform(const std::string& bName, float yaw, float pitch, float roll) {
    auto bdy = bodyList.find(bName);

    if (bdy != bodyList.end()) {
        bdy->second.bodyYawVel = yaw;
        bdy->second.bodyPitchVel = pitch;
        bdy->second.bodyRollVel = roll;
    } else {
        std::cout << "CBodyBase::setBodyTranform(): ERROR! Can't find body - " << bName
                  << std::endl;
    }
}

void CBodyBase::updateBody(const std::string& bName, float dt) {
    auto bdy = bodyList.find(bName);

    if (bdy != bodyList.end()) {
        vec3 offst = vec3Scale(bdy->second.bodyVel, dt);
        bdy->second.bodyPos = vec3Sum(bdy->second.bodyPos, offst);
        bdy->second.bodyYaw += bdy->second.bodyYawVel * dt;
        bdy->second.bodyPitch += bdy->second.bodyPitchVel * dt;
        bdy->second.bodyRoll += bdy->second.bodyRollVel * dt;
    } else {
        std::cout << "CBodyBase::updateBody(): ERROR! Can't find body - " << bName << std::endl;
    }
}

void CBodyBase::renderBody(const std::string& bName) {
    auto bdy = bodyList.find(bName);

    if (bdy != bodyList.end()) {
        mtrx4 mRotate
          = mtrx4FromEuler(bdy->second.bodyYaw, bdy->second.bodyPitch, bdy->second.bodyRoll);
        mtrx4 mOffset = mtrx4FromOffset(bdy->second.bodyPos);

        mtrx4 mAffine = mRotate * mOffset;

        // Прямая транформация в нужное место и с нужным поворотом
        for (size_t i = 0; i < bdy->second.bodyTriangles.size() / 3; i++) {
            bdy->second.bodyTriangles[i * 3 + 0]
              = mtrx4MultVec3(mAffine, bdy->second.bodyTriangles[i * 3 + 0]);
            bdy->second.bodyTriangles[i * 3 + 1]
              = mtrx4MultVec3(mAffine, bdy->second.bodyTriangles[i * 3 + 1]);
            bdy->second.bodyTriangles[i * 3 + 2]
              = mtrx4MultVec3(mAffine, bdy->second.bodyTriangles[i * 3 + 2]);

            bdy->second.bodyNormals[i * 3 + 0]
              = mtrx4MultVec3(mRotate, bdy->second.bodyNormals[i * 3 + 0]);
            bdy->second.bodyNormals[i * 3 + 1]
              = mtrx4MultVec3(mRotate, bdy->second.bodyNormals[i * 3 + 1]);
            bdy->second.bodyNormals[i * 3 + 2]
              = mtrx4MultVec3(mRotate, bdy->second.bodyNormals[i * 3 + 2]);
        }

        // glColor3f(0.3f, 0.6f, 1.0f);
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);

        glVertexPointer(3, GL_FLOAT, 0, &bdy->second.bodyTriangles[0]);
        glNormalPointer(GL_FLOAT, 0, bdy->second.bodyNormals.data());
        glTexCoordPointer(2, GL_FLOAT, 0, &bdy->second.bodyTexCoords[0]);

        glDrawArrays(GL_TRIANGLES, 0, bdy->second.bodyTriangles.size());

        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);

        mAffine.invertSelf();
        mRotate.invertSelf();

        // Обратная трансформация
        for (size_t i = 0; i < bdy->second.bodyTriangles.size() / 3; i++) {
            bdy->second.bodyTriangles[i * 3 + 0]
              = mtrx4MultVec3(mAffine, bdy->second.bodyTriangles[i * 3 + 0]);
            bdy->second.bodyTriangles[i * 3 + 1]
              = mtrx4MultVec3(mAffine, bdy->second.bodyTriangles[i * 3 + 1]);
            bdy->second.bodyTriangles[i * 3 + 2]
              = mtrx4MultVec3(mAffine, bdy->second.bodyTriangles[i * 3 + 2]);

            bdy->second.bodyNormals[i * 3 + 0]
              = mtrx4MultVec3(mRotate, bdy->second.bodyNormals[i * 3 + 0]);
            bdy->second.bodyNormals[i * 3 + 1]
              = mtrx4MultVec3(mRotate, bdy->second.bodyNormals[i * 3 + 1]);
            bdy->second.bodyNormals[i * 3 + 2]
              = mtrx4MultVec3(mRotate, bdy->second.bodyNormals[i * 3 + 2]);
        }
    } else {
        std::cout << "CBodyBase::renderBody(): ERROR! Can't find body - " << bName << std::endl;
    }
}

std::string CBodyBase::getMtrlName(const std::string& bName) {
    auto bdy = bodyList.find(bName);

    return bdy->second.mtrlName;
}

std::vector<std::string> CBodyBase::getEntireBodyQueue() {
    std::vector<std::string> rt;

    for (const auto& bdy : bodyList) {
        rt.push_back(bdy.first);
    }

    return rt;
}
