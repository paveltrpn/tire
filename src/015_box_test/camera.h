
#ifndef __camera_h_
#define __camera_h_

#include "mtrx4.h"
#include "vec3.h"

class CPerspCamera {
    public:
        CPerspCamera() {
            cmrFov = 45.0f;
            cmrAspect = 4.0f / 3.0f;
            cmrNear = 0.1;
            cmrFar = 100.0f;

            cmrYaw = cmrPitch = cmrRoll = 0.0f;

            cmrPosition = vec3();

            cmrViewMatrix = mtrx4FromPerspective(cmrFov, cmrAspect, cmrNear, cmrFar);
        };

        ~CPerspCamera(){};

        void setViewParameters(float fov, float aspect, float near, float far);
        void updateViewMatrix();
        float* getCmrMatrixPointer();

        void setCameraAngles(float yaw, float pitch, float roll);
        void setCameraPosition(const vec3& pos);

        void moveCamera(const vec3& offset);
        void rotateCamera(float yaw, float pitch, float roll);

    private:
        mtrx4 cmrViewMatrix;

        float cmrFov, cmrAspect, cmrNear, cmrFar;
        vec3 cmrPosition;
        float cmrYaw, cmrPitch, cmrRoll;
};

class CPerspLookAtCamera {
    public:
        CPerspLookAtCamera() {
            cmrFov = 45.0f;
            cmrAspect = 4.0f / 3.0f;
            cmrNear = 0.1;
            cmrFar = 100.0f;

            cmrEye = vec3(0.0f, 0.0f, 5.0f);
            cmrTarget = vec3(0.0f, 0.0f, 0.0f);

            cmrUp = vec3(0.0f, 1.0f, 0.0f);

            calculateMoveVectors();

            mtrx4 perspMatrix = mtrx4FromPerspective(cmrFov, cmrAspect, cmrNear, cmrFar);
            mtrx4 lookAtMatrix = mtrx4FromLookAt(cmrEye, cmrTarget, cmrUp);

            cmrViewMatrix = perspMatrix * lookAtMatrix;
        };

        ~CPerspLookAtCamera(){};

        void calculateMoveVectors();

        void setViewParameters(float fov, float aspect, float near, float far);
        void updateViewMatrix();
        void setLookPoints(vec3 eye, vec3 target);
        void setUpVec(const vec3& up);
        void moveViewPointsSideway(float spd);
        void moveViewPointsForward(float spd);
        void rotateEyeUp(float angl);

        float* getCmrMatrixPointer();

    private:
        mtrx4 cmrViewMatrix;

        vec3 cmrEye, cmrTarget, cmrUp;
        vec3 cmrForwardVec, cmrLeftVec;

        float cmrFov, cmrAspect, cmrNear, cmrFar;
};

#endif