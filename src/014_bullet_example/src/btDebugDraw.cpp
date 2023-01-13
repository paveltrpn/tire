
#include "btDebugDraw.h"

#include <array>

#include "glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>

CDebugDraw::CDebugDraw() {
    glLineWidth(5.0f);
}

CDebugDraw::~CDebugDraw() {

}

void CDebugDraw::drawLine(const btVector3& from,const btVector3& to,const btVector3& color)
{
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINES);
        glVertex3fv(&from[0]);
        glVertex3fv(&to[0]);
    glEnd();
}

void CDebugDraw::drawContactPoint(const btVector3& PointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color) {

}

void CDebugDraw::reportErrorWarning(const char* warningString) {
    std::cout << warningString << std::endl;
}

void CDebugDraw::draw3dText(const btVector3& location,const char* textString) {

}

void CDebugDraw::setDebugMode(int debugMode) {
     m_debugMode = (DebugDrawModes)debugMode;
}

int CDebugDraw::getDebugMode() const {
    return m_debugMode;
}