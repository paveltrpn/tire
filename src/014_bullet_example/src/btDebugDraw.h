#pragma once

#include <GL/gl.h>
#include <GL/glew.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>
#include <LinearMath/btIDebugDraw.h>
#include <iostream>
#include <vector>

class CDebugDraw : public btIDebugDraw {
    private:
        int m_debugMode;

    public:
        CDebugDraw();
        virtual ~CDebugDraw(void);

        virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);

        virtual void drawContactPoint(const btVector3& PointOnB,
                                      const btVector3& normalOnB,
                                      btScalar distance,
                                      int lifeTime,
                                      const btVector3& color);

        virtual void reportErrorWarning(const char* warningString);

        virtual void draw3dText(const btVector3& location, const char* textString);

        virtual void setDebugMode(int debugMode);

        virtual int getDebugMode() const;
};