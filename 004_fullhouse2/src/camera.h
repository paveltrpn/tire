
#ifndef __camera_h_
#define __camera_h_

#include "mtrx4.h"
#include "vec3.h"

class Camera {
    public:
        virtual void updateRotation();
        virtual void updatePosition();

        virtual void updateViewMatrix() = 0;

        virtual void setCameraPosition(const vec3 &pos);
        virtual void setCameraAngles(float yaw, float pitch, float roll);

        virtual float* getCmrMatrixPointer();

    protected:
        mtrx4 cmrViewMatrix;
        mtrx4 cmrRotateMatrix;
        mtrx4 cmrOffsetMatrix;
        vec3  cmrPosition;

        float cmrYaw, cmrPitch, cmrRoll;
};

class CameraOrtho: Camera {
    public:
        CameraOrtho() {
            
        };

        ~CameraOrtho() {};
};

class CameraPersp: public Camera {
    public:
        CameraPersp() {
            cmrFov = 45.0f;
            cmrAspect = 4.0f / 3.0f;
            cmrNear = 0.1;
            cmrFar = 100.0f;

            cmrYaw = cmrPitch = cmrRoll = 0.0f;

            cmrPosition = vec3();

            cmrRotateMatrix = mtrx4();
            cmrOffsetMatrix = mtrx4();

            cmrViewMatrix = mtrx4FromPerspective(cmrFov, cmrAspect, cmrNear, cmrFar);
        };

        ~CameraPersp() {};

        void setViewParameters(float fov, float aspect, float near, float far);
        void updateViewMatrix();

    private:
        float cmrFov, cmrAspect, cmrNear, cmrFar;
};

#endif