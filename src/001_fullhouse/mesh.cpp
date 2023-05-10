
#include <array>
#include <tuple>

#include "mesh.h"
#include "qtnn.h"

using namespace std;

vec3 x_ax = vec3(1.0f, 0.0f, 0.0f);
vec3 y_ax = vec3(0.0f, 1.0f, 0.0f);
vec3 z_ax = vec3(0.0f, 0.0f, 1.0f);
mtrx3 m_rtn;
qtnn q_rtn;

void draw_decart(float scale) {
    glBegin(GL_LINES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(scale, 0.0f, 0.0f);
    glEnd();

    glBegin(GL_LINES);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, scale, 0.0f);
    glEnd();

    glBegin(GL_LINES);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, scale);
    glEnd();
}

void box_c::append(const vec3 &pos, const mtrx3 &spd) {
    orientation.push_back({ pos, spd, mtrx3() });
}

void box_c::show() {
    uint8_t j;

    for (auto &it : orientation) {
        for (j = 0; j < 8; j++) {
            /* поворачиваем, умножая вершину на матрицу положения */
            clone[j] = mtrx3MultVec(get<2>(it), base[j]);
            /* переносим, склалдывая вершину с вектором смещения */
            clone[j] = vec3Sum(get<0>(it), clone[j]);
        }

        for (j = 0; j < 6; j++) {
            /* поворачиваем, умножая вершину на матрицу положения */
            clone_normal[j] = mtrx3MultVec(get<2>(it), base_normal[j]);
        }

        /* изменяем положение, умножая матрицу положения на матрицу скорости вращения */
        get<2>(it) = mtrx3Mult(get<2>(it), get<1>(it));

        glBegin(GL_QUADS);

        glColor3f(0.1f, 0.6f, 0.1f);
        glNormal3fv(&clone_normal[0][0]);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3fv(&clone[0][0]);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3fv(&clone[1][0]);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3fv(&clone[2][0]);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3fv(&clone[3][0]);

        glColor3f(0.8f, 0.9f, 0.1f);
        glNormal3fv(&clone_normal[1][0]);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3fv(&clone[4][0]);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3fv(&clone[5][0]);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3fv(&clone[6][0]);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3fv(&clone[7][0]);

        glColor3f(0.0f, 1.0f, 1.0f);
        glNormal3fv(&clone_normal[2][0]);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3fv(&clone[0][0]);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3fv(&clone[1][0]);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3fv(&clone[5][0]);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3fv(&clone[4][0]);

        glColor3f(0.93f, 0.11f, 0.23f);
        glNormal3fv(&clone_normal[3][0]);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3fv(&clone[2][0]);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3fv(&clone[3][0]);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3fv(&clone[7][0]);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3fv(&clone[6][0]);

        glColor3f(0.1f, 0.15f, 0.75f);
        glNormal3fv(&clone_normal[4][0]);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3fv(&clone[0][0]);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3fv(&clone[4][0]);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3fv(&clone[7][0]);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3fv(&clone[3][0]);

        glColor3f(0.8f, 0.2f, 0.98f);
        glNormal3fv(&clone_normal[5][0]);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3fv(&clone[1][0]);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3fv(&clone[5][0]);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3fv(&clone[6][0]);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3fv(&clone[2][0]);

        glEnd();
    }
}

std::tuple<std::array<vec3, 3>, vec3> bodyBases
  = { { { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } } },
      { 0.0f, 0.0f, 0.0f } };

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
  = { { 0.0f, 0.0f, 1.0f },  { 0.0f, 0.0f, 1.0f },  { 0.0f, -1.0f, 0.0f }, { 0.0f, -1.0f, 0.0f },
      { -1.0f, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f, -1.0f },
      { 1.0f, 0.0f, 0.0f },  { 1.0f, 0.0f, 0.0f },  { 0.0f, 1.0f, 0.0f },  { 0.0f, 1.0f, 0.0f } };

std::vector<vec3> prismTris
  = { { 0.000000, -0.000000, -1.000000 },     { 0.000000, 0.866025, -0.500000 },
      { 0.750000, -0.433013, -0.500000 },     { 0.000000, 0.866025, -0.500000 },
      { 0.000000, 0.866025, 0.500000 },       { 0.750000, -0.433013, 0.500000 },
      { 0.000000, 0.866025, -0.500000 },      { 0.750000, -0.433013, 0.500000 },
      { 0.750000, -0.433013, -0.500000 },     { 0.000000, 0.866025, 0.500000 },
      { -0.000000, -0.000000, 1.000000 },     { 0.750000, -0.433013, 0.500000 },
      { 0.000000, -0.000000, -1.000000 },     { 0.750000, -0.433013, -0.500000 },
      { -0.750000, -0.433013, -0.500000 },    { 0.750000, -0.433013, -0.500000 },
      { 0.750000, -0.433013, 0.500000 },      { -0.750000, -0.433013, 0.500000 },
      { 0.750000f, -0.433013f, -0.500000f },  { -0.750000f, -0.433013f, 0.500000f },
      { -0.750000f, -0.433013f, -0.500000f }, { 0.750000f, -0.433013f, 0.500000f },
      { -0.000000f, -0.000000f, 1.000000f },  { -0.750000f, -0.433013f, 0.500000f },
      { 0.000000f, -0.000000f, -1.000000f },  { -0.750000f, -0.433013f, -0.500000f },
      { 0.000000f, 0.866025f, -0.500000f },   { -0.750000f, -0.433013f, -0.500000f },
      { -0.750000f, -0.433013f, 0.500000f },  { 0.000000f, 0.866025f, 0.500000f },
      { -0.750000f, -0.433013f, -0.500000f }, { 0.000000f, 0.866025f, 0.500000f },
      { 0.000000f, 0.866025f, -0.500000f },   { -0.750000f, -0.433013f, 0.500000f },
      { -0.000000f, -0.000000f, 1.000000f },  { 0.000000f, 0.866025f, 0.500000f } };

std::vector<vec3> prismNrmls
  = { { 0.654654, 0.377964, -0.654654 },     { 0.866026, 0.500000, -0.000000 },
      { 0.866025, 0.500000, 0.000000 },      { 0.654653, 0.377964, 0.654654 },
      { -0.000000, -0.755929, -0.654654 },   { -0.000000, -1.000000, -0.000000 },
      { -0.000000, -1.000000, 0.000000 },    { -0.000000f, -0.755929f, 0.654654f },
      { -0.654653f, 0.377964f, -0.654654f }, { -0.866025f, 0.500000f, 0.000000f },
      { -0.866025f, 0.500000f, -0.000000 },  { -0.654654f, 0.377964f, 0.654654f } };

std::vector<vec3> icosahedrTris
  = { { 0.000000f, 0.000000f, -1.000000f },   { 0.723600f, -0.525720f, -0.447215f },
      { -0.276385f, -0.850640f, -0.447215f }, { 0.723600f, -0.525720f, -0.447215f },
      { 0.000000f, 0.000000f, -1.000000f },   { 0.723600f, 0.525720f, -0.447215f },
      { 0.000000f, 0.000000f, -1.000000f },   { -0.276385f, -0.850640f, -0.447215f },
      { -0.894425f, 0.000000f, -0.447215f },  { 0.000000f, 0.000000f, -1.000000f },
      { -0.894425f, 0.000000f, -0.447215f },  { -0.276385f, 0.850640f, -0.447215f },
      { 0.000000f, 0.000000f, -1.000000f },   { -0.276385f, 0.850640f, -0.447215f },
      { 0.723600f, 0.525720f, -0.447215f },   { 0.723600f, -0.525720f, -0.447215f },
      { 0.723600f, 0.525720f, -0.447215f },   { 0.894425f, 0.000000f, 0.447215f },
      { -0.276385f, -0.850640f, -0.447215f }, { 0.723600f, -0.525720f, -0.447215f },
      { 0.276385f, -0.850640f, 0.447215f },   { -0.894425f, 0.000000f, -0.447215f },
      { -0.276385f, -0.850640f, -0.447215f }, { -0.723600f, -0.525720f, 0.447215f },
      { -0.276385f, 0.850640f, -0.447215f },  { -0.894425f, 0.000000f, -0.447215f },
      { -0.723600f, 0.525720f, 0.447215f },   { 0.723600f, 0.525720f, -0.447215f },
      { -0.276385f, 0.850640f, -0.447215f },  { 0.276385f, 0.850640f, 0.447215f },
      { 0.723600f, -0.525720f, -0.447215f },  { 0.894425f, 0.000000f, 0.447215f },
      { 0.276385f, -0.850640f, 0.447215f },   { -0.276385f, -0.850640f, -0.447215f },
      { 0.276385f, -0.850640f, 0.447215f },   { -0.723600f, -0.525720f, 0.447215f },
      { -0.894425f, 0.000000f, -0.447215f },  { -0.723600f, -0.525720f, 0.447215f },
      { -0.723600f, 0.525720f, 0.447215f },   { -0.276385f, 0.850640f, -0.447215f },
      { -0.723600f, 0.525720f, 0.447215f },   { 0.276385f, 0.850640f, 0.447215f },
      { 0.723600f, 0.525720f, -0.447215f },   { 0.276385f, 0.850640f, 0.447215f },
      { 0.894425f, 0.000000f, 0.447215f },    { 0.276385f, -0.850640f, 0.447215f },
      { 0.894425f, 0.000000f, 0.447215f },    { 0.000000f, 0.000000f, 1.000000f },
      { -0.723600f, -0.525720f, 0.447215f },  { 0.276385f, -0.850640f, 0.447215f },
      { 0.000000f, 0.000000f, 1.000000f },    { -0.723600f, 0.525720f, 0.447215f },
      { -0.723600f, -0.525720f, 0.447215f },  { 0.000000f, 0.000000f, 1.000000f },
      { 0.276385f, 0.850640f, 0.447215f },    { -0.723600f, 0.525720f, 0.447215f },
      { 0.000000f, 0.000000f, 1.000000f },    { 0.894425f, 0.000000f, 0.447215f },
      { 0.276385f, 0.850640f, 0.447215f },    { 0.000000f, 0.000000f, 1.000000f } };

std::vector<vec3> icosahedrNrmls
  = { { 0.187597f, -0.577354f, -0.794651 },   { 0.607065f, 0.000000f, -0.794652f },
      { -0.491122f, -0.356829f, -0.794652f }, { -0.491122f, 0.356829f, -0.794652f },
      { 0.187597f, 0.577354f, -0.794651f },   { 0.982246f, 0.000000f, -0.187597f },
      { 0.303536f, -0.934171f, -0.187589f },  { -0.794649f, -0.577359f, -0.187587f },
      { -0.794649f, 0.577359f, -0.187587f },  { 0.303536f, 0.934171f, -0.187589f },
      { 0.794649f, -0.577359f, 0.187587f },   { -0.303536f, -0.934171f, 0.187589f },
      { -0.982246f, 0.000000f, 0.187597f },   { -0.303536f, 0.934171f, 0.187589f },
      { 0.794649, 0.577359f, 0.187587f },     { 0.491122f, -0.356829f, 0.794652f },
      { -0.187596f, -0.577354f, 0.794651f },  { -0.607065f, 0.000000f, 0.794652f },
      { -0.187597f, 0.577354f, 0.794651f },   { 0.491122f, 0.356829f, 0.794652f } };

Body::Body(int type, vec3 offst, vec3 orn, vec3 scl) {
    BodyOrientation = orn;
    BodyOffset = offst;
    BodyScale = scl;

    switch (type) {
    case Body::ICOSAHEDRON:
        BodyTriangles = icosahedrTris;
        BodyNormals = icosahedrNrmls;
        break;

    case Body::BOX:
        BodyTriangles = boxTris;
        BodyNormals = boxNrmls;
        break;

    case Body::PRISM:
        BodyTriangles = prismTris;
        BodyNormals = prismNrmls;
        break;
    }
}

Body::~Body() {
}

void Body::updateAndDraw() {
    vec3 cur_tri[3];
    vec3 cur_nrml;

    mtrx4 mRotate = mtrx4FromEuler(BodyOrientation[0], BodyOrientation[1], BodyOrientation[2]);
    mtrx4 mOffset = mtrx4FromOffset(BodyOffset);
    mtrx4 mScale = mtrx4FromScale(BodyScale);

    // mtrx4 tmp = mtrx4Mult(mScale, mRotate);
    // mtrx4 mAffine = mtrx4Mult(tmp, mOffset);

    mtrx4 mAffine = mScale * mRotate * mOffset;

    for (size_t i = 0; i < BodyTriangles.size() / 3; i++) {
        cur_tri[0] = mtrx4MultVec3(mAffine, BodyTriangles[i * 3 + 0]);
        cur_tri[1] = mtrx4MultVec3(mAffine, BodyTriangles[i * 3 + 1]);
        cur_tri[2] = mtrx4MultVec3(mAffine, BodyTriangles[i * 3 + 2]);

        cur_nrml = mtrx4MultVec3(mRotate, BodyNormals[i]);

        glBegin(GL_TRIANGLES);
        glNormal3fv(&cur_nrml[0]);
        glVertex3fv(&cur_tri[0][0]);
        glVertex3fv(&cur_tri[1][0]);
        glVertex3fv(&cur_tri[2][0]);
        glEnd();
    }
}

void Body::setOrientation(float yaw, float pitch, float roll) {
    BodyOrientation = vec3(yaw, pitch, roll);
}

void Body::setOffset(float dx, float dy, float dz) {
    BodyOffset = vec3(dx, dy, dz);
}