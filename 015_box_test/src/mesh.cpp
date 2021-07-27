
#include <tuple>
#include <array>

#include "mesh.h"
#include "qtnn.h"
#include "apputils.h"

// По три нормали на вершину!!!
std::vector<vec3> boxTris = 	{{1.0f, 1.0f, 1.0f},  {-1.0f, 1.0f, 1.0f},  {-1.0f, -1.0f, 1.0f},
								{ 1.0f,  1.0f, 1.0f}, {-1.0f, -1.0f, 1.0f}, { 1.0f, -1.0f, 1.0f},
								{ 1.0f, -1.0f, -1.0f}, { 1.0f, -1.0f,  1.0f}, {-1.0f, -1.0f,  1.0f},
								{ 1.0f, -1.0f, -1.0f}, {-1.0f, -1.0f,  1.0f}, {-1.0f, -1.0f, -1.0f},
								{-1.0f, -1.0,-1.0}, {-1.0f, -1.0, 1.0}, {-1.0f,  1.0, 1.0},
								{-1.0f, -1.0f, -1.0}, {-1.0f,  1.0f,  1.0}, {-1.0f,  1.0f, -1.0},
								{-1.0f, 1.0f, -1.0f}, { 1.0f, 1.0f, -1.0f}, { 1.0f,-1.0f, -1.0f},
								{-1.0f,  1.0f, -1.0}, { 1.0f, -1.0f, -1.0}, {-1.0f, -1.0f, -1.0},
								{1.0f, 1.0f,-1.0}, {1.0f, 1.0f, 1.0}, {1.0f,-1.0f, 1.0},
								{1.0f,  1.0f, -1.0}, {1.0f, -1.0f,  1.0}, {1.0f, -1.0f, -1.0},
								{-1.0f, 1.0f,-1.0f}, {-1.0f, 1.0f, 1.0f}, { 1.0f, 1.0f, 1.0f},
								{-1.0f, 1.0, -1.0f}, { 1.0f, 1.0,  1.0f}, { 1.0f, 1.0, -1.0f}};

std::vector<vec3> boxNrmls = 	{{0.0f, 0.0f, 1.0f},{0.0f, 0.0f, 1.0f},{0.0f, 0.0f, 1.0f},
								{0.0f, 0.0f, 1.0f},{0.0f, 0.0f, 1.0f},{0.0f, 0.0f, 1.0f},
								{0.0f,-1.0f, 0.0f},{0.0f,-1.0f, 0.0f},{0.0f,-1.0f, 0.0f},
								{0.0f,-1.0f, 0.0f},{0.0f,-1.0f, 0.0f},{0.0f,-1.0f, 0.0f},
								{-1.0f,0.0f, 0.0f},{-1.0f,0.0f, 0.0f},{-1.0f,0.0f, 0.0f},
								{-1.0f,0.0f, 0.0f},{-1.0f,0.0f, 0.0f},{-1.0f,0.0f, 0.0f},
								{0.0f, 0.0f,-1.0f},{0.0f, 0.0f,-1.0f},{0.0f, 0.0f,-1.0f},
								{0.0f, 0.0f,-1.0f},{0.0f, 0.0f,-1.0f},{0.0f, 0.0f,-1.0f},
								{1.0f, 0.0f, 0.0f},{1.0f, 0.0f, 0.0f},{1.0f, 0.0f, 0.0f},
								{1.0f, 0.0f, 0.0f},{1.0f, 0.0f, 0.0f},{1.0f, 0.0f, 0.0f},
								{0.0f, 1.0f, 0.0f},{0.0f, 1.0f, 0.0f},{0.0f, 1.0f, 0.0f},
								{0.0f, 1.0f, 0.0f},{0.0f, 1.0f, 0.0f},{0.0f, 1.0f, 0.0f}};

std::vector<vec2> boxTexCoords = {{0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f},
								  {0.0f, 0.0f}, {1.0f, 1.0f}, {1.0f, 0.0f},
								  {0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f},
								  {0.0f, 0.0f}, {1.0f, 1.0f}, {1.0f, 0.0f},
								  {0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f},
								  {0.0f, 0.0f}, {1.0f, 1.0f}, {1.0f, 0.0f},
								  {0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f},
								  {0.0f, 0.0f}, {1.0f, 1.0f}, {1.0f, 0.0f},
								  {0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f},
								  {0.0f, 0.0f}, {1.0f, 1.0f}, {1.0f, 0.0f},
								  {0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f},
								  {0.0f, 0.0f}, {1.0f, 1.0f}, {1.0f, 0.0f}};

BasicBody::BasicBody() {
	m_bodyYaw = m_bodyPitch = m_bodyRoll = 0.0f;
    m_bodyOffset = vec3();
		
	BodyTriangles = boxTris;
	BodyNormals = boxNrmls;
	BodyTexCoords = boxTexCoords;
}

BasicBody::BasicBody(vec3 scl) {
	m_bodyYaw = m_bodyPitch = m_bodyRoll = 0.0f;
    m_bodyOffset = vec3();

	BodyTriangles = boxTris;
	BodyNormals = boxNrmls;
	BodyTexCoords = boxTexCoords;

	mtrx4 scaleMtrx = mtrx4FromScale(scl);
	for (auto &vert: BodyTriangles) {
		vert = mtrx4MultVec3(scaleMtrx, vert);
	}

	// glGenBuffers(1, &m_oglBuffer);
	// glBindBuffer(GL_ARRAY_BUFFER, m_oglBuffer);
	// glBufferData(GL_ARRAY_BUFFER, BodyTriangles.size()*3, 0, GL_STATIC_DRAW); // reserve space
}

BasicBody::~BasicBody() {
	// glDeleteBuffers(1, &m_oglBuffer);
}

void BasicBody::updateAndDraw() {
	mtrx4 mRotate = mtrx4FromEuler(m_bodyYaw, m_bodyPitch, m_bodyRoll);
	mtrx4 mOffset = mtrx4FromOffset(m_bodyOffset);
	
	mtrx4 mAffine = mRotate * mOffset;

	// Прямая транформация в нужное место и с нужным поворотом
	for (size_t i = 0; i < BodyTriangles.size()/3; i++ ) {
		BodyTriangles[i*3+0] = mtrx4MultVec3(mAffine, BodyTriangles[i*3+0]);
		BodyTriangles[i*3+1] = mtrx4MultVec3(mAffine, BodyTriangles[i*3+1]);
		BodyTriangles[i*3+2] = mtrx4MultVec3(mAffine, BodyTriangles[i*3+2]);
		
		BodyNormals[i*3+0] = mtrx4MultVec3(mRotate, BodyNormals[i*3+0]);
		BodyNormals[i*3+1] = mtrx4MultVec3(mRotate, BodyNormals[i*3+1]);
		BodyNormals[i*3+2] = mtrx4MultVec3(mRotate, BodyNormals[i*3+2]);
	}

	// glColor3f(0.3f, 0.6f, 1.0f);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, 	&BodyTriangles[0]);
	glNormalPointer(GL_FLOAT, 0, 		BodyNormals.data());
	glTexCoordPointer(2, GL_FLOAT, 0, 	&BodyTexCoords[0]);

	glDrawArrays(GL_TRIANGLES, 0, BodyTriangles.size());
	
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	
	mAffine.invertSelf();
	mRotate.invertSelf();

	// Обратная трансформация
	for (size_t i = 0; i < BodyTriangles.size()/3; i++ ) {
		BodyTriangles[i*3+0] = mtrx4MultVec3(mAffine, BodyTriangles[i*3+0]);
		BodyTriangles[i*3+1] = mtrx4MultVec3(mAffine, BodyTriangles[i*3+1]);
		BodyTriangles[i*3+2] = mtrx4MultVec3(mAffine, BodyTriangles[i*3+2]);
		
		BodyNormals[i*3+0] = mtrx4MultVec3(mRotate, BodyNormals[i*3+0]);
		BodyNormals[i*3+1] = mtrx4MultVec3(mRotate, BodyNormals[i*3+1]);
		BodyNormals[i*3+2] = mtrx4MultVec3(mRotate, BodyNormals[i*3+2]);
	}
}

void BasicBody::setOrientation(float yaw, float pitch, float roll) {
	m_bodyYaw = yaw;
	m_bodyPitch = pitch;
	m_bodyRoll = roll;
}

void BasicBody::setOffset(vec3 offst){
	m_bodyOffset = offst;
}

void BasicBody::bodyMove(vec3 offst) {
	m_bodyOffset = vec3Sum(m_bodyOffset, offst);
}

void BasicBody::bodyRotate(float yaw, float pitch, float roll) {
	m_bodyYaw += yaw;
	m_bodyPitch += pitch;
	m_bodyRoll += roll;
}
