
#include <tuple>
#include <array>

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

#include "apputils.h"

#include "mesh.h"

// По три нормали на вершину!!!

std::vector<glm::vec3> boxTris 	{glm::vec3(1.0f,  1.0f,  1.0f),  glm::vec3(-1.0f, 1.0f, 1.0f),  glm::vec3(-1.0f, -1.0f, 1.0f),
									 glm::vec3( 1.0f,  1.0f,  1.0f), glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3( 1.0f, -1.0f, 1.0f),
									 glm::vec3( 1.0f, -1.0f, -1.0f), glm::vec3( 1.0f, -1.0f,  1.0f), glm::vec3(-1.0f, -1.0f,  1.0f),
									 glm::vec3( 1.0f, -1.0f, -1.0f), glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec3(-1.0f, -1.0f, -1.0f),
									 glm::vec3(-1.0f, -1.0,  -1.0f), glm::vec3(-1.0f, -1.0, 1.0), glm::vec3(-1.0f,  1.0, 1.0),
									 glm::vec3(-1.0f, -1.0f, -1.0), glm::vec3(-1.0f,  1.0f,  1.0), glm::vec3(-1.0f,  1.0f, -1.0),
									 glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec3( 1.0f, 1.0f, -1.0f), glm::vec3( 1.0f,-1.0f, -1.0f),
									 glm::vec3(-1.0f,  1.0f, -1.0), glm::vec3( 1.0f, -1.0f, -1.0), glm::vec3(-1.0f, -1.0f, -1.0),
									 glm::vec3(1.0f, 1.0f,-1.0), glm::vec3(1.0f, 1.0f, 1.0), glm::vec3(1.0f,-1.0f, 1.0),
									 glm::vec3(1.0f,  1.0f, -1.0), glm::vec3(1.0f, -1.0f,  1.0), glm::vec3(1.0f, -1.0f, -1.0),
									 glm::vec3(-1.0f, 1.0f,-1.0f), glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec3( 1.0f, 1.0f, 1.0f),
									 glm::vec3(-1.0f, 1.0, -1.0f), glm::vec3( 1.0f, 1.0,  1.0f), glm::vec3( 1.0f, 1.0, -1.0f)};

std::vector<glm::vec3> boxNrmls {glm::vec3(0.0f, 0.0f, 1.0f),glm::vec3(0.0f, 0.0f, 1.0f),glm::vec3(0.0f, 0.0f, 1.0f),
								glm::vec3(0.0f, 0.0f, 1.0f),glm::vec3(0.0f, 0.0f, 1.0f),glm::vec3(0.0f, 0.0f, 1.0f),
								glm::vec3(0.0f,-1.0f, 0.0f),glm::vec3(0.0f,-1.0f, 0.0f),glm::vec3(0.0f,-1.0f, 0.0f),
								glm::vec3(0.0f,-1.0f, 0.0f),glm::vec3(0.0f,-1.0f, 0.0f),glm::vec3(0.0f,-1.0f, 0.0f),
								glm::vec3(-1.0f,0.0f, 0.0f),glm::vec3(-1.0f,0.0f, 0.0f),glm::vec3(-1.0f,0.0f, 0.0f),
								glm::vec3(-1.0f,0.0f, 0.0f),glm::vec3(-1.0f,0.0f, 0.0f),glm::vec3(-1.0f,0.0f, 0.0f),
								glm::vec3(0.0f, 0.0f,-1.0f),glm::vec3(0.0f, 0.0f,-1.0f),glm::vec3(0.0f, 0.0f,-1.0f),
								glm::vec3(0.0f, 0.0f,-1.0f),glm::vec3(0.0f, 0.0f,-1.0f),glm::vec3(0.0f, 0.0f,-1.0f),
								glm::vec3(1.0f, 0.0f, 0.0f),glm::vec3(1.0f, 0.0f, 0.0f),glm::vec3(1.0f, 0.0f, 0.0f),
								glm::vec3(1.0f, 0.0f, 0.0f),glm::vec3(1.0f, 0.0f, 0.0f),glm::vec3(1.0f, 0.0f, 0.0f),
								glm::vec3(0.0f, 1.0f, 0.0f),glm::vec3(0.0f, 1.0f, 0.0f),glm::vec3(0.0f, 1.0f, 0.0f),
								glm::vec3(0.0f, 1.0f, 0.0f),glm::vec3(0.0f, 1.0f, 0.0f),glm::vec3(0.0f, 1.0f, 0.0f)};

std::vector<glm::vec2> boxTexCoords {glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec2(1.0f, 1.0f),
								  glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 0.0f),
								  glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec2(1.0f, 1.0f),
								  glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 0.0f),
								  glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec2(1.0f, 1.0f),
								  glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 0.0f),
								  glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec2(1.0f, 1.0f),
								  glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 0.0f),
								  glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec2(1.0f, 1.0f),
								  glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 0.0f),
								  glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec2(1.0f, 1.0f),
								  glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 0.0f)};


CBodyBase::CBodyBase() {

}

CBodyBase::~CBodyBase() {

}

void CBodyBase::appendNewBody(const std::string& bName, const std::string& mtrlName) {
	SBodyStruct tmp;

	tmp.bodyYaw = tmp.bodyPitch = tmp.bodyRoll = 0.0f;
	tmp.bodyYawVel = tmp.bodyPitchVel = tmp.bodyRollVel = 0.0f;
    tmp.bodyPos = glm::vec3();
		
	tmp.bodyTriangles = boxTris;
	tmp.bodyNormals   = boxNrmls;
	tmp.bodyTexCoords = boxTexCoords;

	tmp.mtrlName = mtrlName;

	bodyList.insert({bName, tmp});
}

void CBodyBase::appendNewBody(const std::string& bName, const std::string& mtrlName,const glm::vec3& scl) {
	SBodyStruct tmp;

	tmp.bodyYaw = tmp.bodyPitch = tmp.bodyRoll = 0.0f;
	tmp.bodyYawVel = tmp.bodyPitchVel = tmp.bodyRollVel = 0.0f;
    tmp.bodyPos = glm::vec3();
		
	tmp.bodyTriangles = boxTris;
	tmp.bodyNormals   = boxNrmls;
	tmp.bodyTexCoords = boxTexCoords;

	tmp.mtrlName = mtrlName;

	auto scaleMtrx = glm::scale(glm::mat4(), scl);

	for (auto &vert: tmp.bodyTriangles) {
		auto bar = glm::vec4(vert, 1.0f);
		bar = scaleMtrx * bar;
		vert = bar.swizzle(glm::X, glm::Y, glm::Z);
	}

	bodyList.insert({bName, tmp});
}

void CBodyBase::setBodyParameters(const std::string& bName, const glm::vec3& offst) {
	auto bdy = bodyList.find(bName);

	if (bdy != bodyList.end()) {
		bdy->second.bodyPos = offst;
	} else {
		std::cout << "CBodyBase::setBodyParameters(): ERROR! Can't find body - " << bName << std::endl;
	}
}

void CBodyBase::setBodyParameters(const std::string& bName, float yaw, float pitch, float roll) {
	auto bdy = bodyList.find(bName);

	if (bdy != bodyList.end()) {
		bdy->second.bodyYaw   = yaw;
		bdy->second.bodyPitch = pitch;
		bdy->second.bodyRoll  = roll;
	} else {
		std::cout << "CBodyBase::setBodyParameters(): ERROR! Can't find body - " << bName << std::endl;
	}
}

void CBodyBase::setBodyParameters(const std::string& bName, float yaw, float pitch, float roll, const glm::vec3& offst) {
	auto bdy = bodyList.find(bName);

	if (bdy != bodyList.end()) {
		bdy->second.bodyPos   = offst;
		bdy->second.bodyYaw   = yaw;
		bdy->second.bodyPitch = pitch;
		bdy->second.bodyRoll  = roll;
	} else {
		std::cout << "CBodyBase::setBodyParameters(): ERROR! Can't find body - " << bName << std::endl;
	}
}

void CBodyBase::setBodyTranform(const std::string& bName, float yaw, float pitch, float roll) {
	auto bdy = bodyList.find(bName);

	if (bdy != bodyList.end()) {
		bdy->second.bodyYawVel   = yaw;
		bdy->second.bodyPitchVel = pitch;
		bdy->second.bodyRollVel  = roll;
	} else {
		std::cout << "CBodyBase::setBodyTranform(): ERROR! Can't find body - " << bName << std::endl;
	}
}

void CBodyBase::updateBody(const std::string& bName, float dt) {
	auto bdy = bodyList.find(bName);

	if (bdy != bodyList.end()) {
		glm::vec3 offst = bdy->second.bodyVel * dt;
		bdy->second.bodyPos = bdy->second.bodyPos + offst;
		bdy->second.bodyYaw   += bdy->second.bodyYawVel   * dt;
		bdy->second.bodyPitch += bdy->second.bodyPitchVel * dt;
		bdy->second.bodyRoll  += bdy->second.bodyRollVel  * dt;
	} else {
		std::cout << "CBodyBase::updateBody(): ERROR! Can't find body - " << bName << std::endl;
	}
}

constexpr float degToRad(float deg) {
	return deg * 3.1415f/180.0f;
}

void CBodyBase::renderBody(const std::string& bName) {
	auto bdy = bodyList.find(bName);

	if (bdy != bodyList.end()) {
		glm::mat4 mRotate = glm::eulerAngleYXZ(degToRad(bdy->second.bodyYaw), 
									   		   degToRad(bdy->second.bodyPitch), 
									   		   degToRad(bdy->second.bodyRoll));
		glm::mat4 mOffset = glm::translate(glm::mat4(), bdy->second.bodyPos);

		glm::mat4 mAffine = mOffset * mRotate;

		// Прямая транформация в нужное место и с нужным поворотом
		for (size_t i = 0; i < bdy->second.bodyTriangles.size()/3; i++ ) {
			bdy->second.bodyTriangles[i*3+0] = glm::vec3(mAffine * glm::vec4(bdy->second.bodyTriangles[i*3+0], 1.0f));
			bdy->second.bodyTriangles[i*3+1] = glm::vec3(mAffine * glm::vec4(bdy->second.bodyTriangles[i*3+1], 1.0f));
			bdy->second.bodyTriangles[i*3+2] = glm::vec3(mAffine * glm::vec4(bdy->second.bodyTriangles[i*3+2], 1.0f));

			bdy->second.bodyNormals[i*3+0] = glm::vec3(mRotate * glm::vec4(bdy->second.bodyNormals[i*3+0], 1.0f));
			bdy->second.bodyNormals[i*3+1] = glm::vec3(mRotate * glm::vec4(bdy->second.bodyNormals[i*3+1], 1.0f));
			bdy->second.bodyNormals[i*3+2] = glm::vec3(mRotate * glm::vec4(bdy->second.bodyNormals[i*3+2], 1.0f));
		}

		// glColor3f(0.3f, 0.6f, 1.0f);
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glVertexPointer(3, GL_FLOAT, 0, 	&bdy->second.bodyTriangles[0]);
		glNormalPointer(GL_FLOAT, 0, 		 bdy->second.bodyNormals.data());
		glTexCoordPointer(2, GL_FLOAT, 0, 	&bdy->second.bodyTexCoords[0]);

		glDrawArrays(GL_TRIANGLES, 0, bdy->second.bodyTriangles.size());

		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
	
		mAffine = glm::inverse(mAffine);
		mRotate = glm::inverse(mRotate);

		// Обратная трансформация
		for (size_t i = 0; i < bdy->second.bodyTriangles.size()/3; i++ ) {
			bdy->second.bodyTriangles[i*3+0] = glm::vec3(mAffine * glm::vec4(bdy->second.bodyTriangles[i*3+0], 1.0f));
			bdy->second.bodyTriangles[i*3+1] = glm::vec3(mAffine * glm::vec4(bdy->second.bodyTriangles[i*3+1], 1.0f));
			bdy->second.bodyTriangles[i*3+2] = glm::vec3(mAffine * glm::vec4(bdy->second.bodyTriangles[i*3+2], 1.0f));

			bdy->second.bodyNormals[i*3+0] = glm::vec3(mRotate * glm::vec4(bdy->second.bodyNormals[i*3+0], 1.0f));
			bdy->second.bodyNormals[i*3+1] = glm::vec3(mRotate * glm::vec4(bdy->second.bodyNormals[i*3+1], 1.0f));
			bdy->second.bodyNormals[i*3+2] = glm::vec3(mRotate * glm::vec4(bdy->second.bodyNormals[i*3+2], 1.0f));
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

	for (const auto& bdy: bodyList) {
		rt.push_back(bdy.first);
	}

	return rt;
}
