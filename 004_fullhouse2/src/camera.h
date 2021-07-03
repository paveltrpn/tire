
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

        ~CPerspCamera() {};

        void    setViewParameters(float fov, float aspect, float near, float far);
        void    updateViewMatrix();
        float*  getCmrMatrixPointer();

        void    setCameraAngles(float yaw, float pitch, float roll);
        void    setCameraPosition(const vec3 &pos);

        void    moveCamera(const vec3 &offset);
        void    rotateCamera(float yaw, float pitch, float roll);

    private:
        mtrx4 cmrViewMatrix;

        float cmrFov, cmrAspect, cmrNear, cmrFar;
        vec3  cmrPosition;
        float cmrYaw, cmrPitch, cmrRoll;
};

#endif