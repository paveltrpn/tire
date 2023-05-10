
#ifndef __camera_h_
#define __camera_h_

#include "mtrx4.h"
#include "vec3.h"

class Camera {
    public:
        enum { ORTHO, PERSPECTIVE };

        Camera() {
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

        ~Camera(){};

        void updateRotation();
        void updatePosition();
        void updateViewParameters();

        void setViewParameters(float fov, float aspect, float near, float far);

        void setCameraPosition(const vec3& pos);
        void setCameraAngles(float yaw, float pitch, float roll);

        float* getCmrMatrixPointer();

    private:
        mtrx4 cmrViewMatrix;
        mtrx4 cmrRotateMatrix;
        mtrx4 cmrOffsetMatrix;

        vec3 cmrPosition;

        float cmrFov, cmrAspect, cmrNear, cmrFar;

        float cmrYaw, cmrPitch, cmrRoll;
};

#endif