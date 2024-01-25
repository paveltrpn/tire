
#ifndef __camera_h_
#define __camera_h_

import toy_std;

namespace alg = toy::algebra;

class Camera {
    public:
        enum { ORTHO, PERSPECTIVE };

        Camera() {
            cmrFov = 45.0f;
            cmrAspect = 4.0f / 3.0f;
            cmrNear = 0.1;
            cmrFar = 100.0f;

            cmrYaw = cmrPitch = cmrRoll = 0.0f;

            cmrPosition = alg::vector3f{};

            cmrRotateMatrix = alg::matrix4f{};
            cmrOffsetMatrix = alg::matrix4f{};

            cmrViewMatrix.set_perspective(cmrFov, cmrAspect, cmrNear, cmrFar);
        };

        ~Camera(){};

        void updateRotation();
        void updatePosition();
        void updateViewParameters();

        void setViewParameters(float fov, float aspect, float near, float far);

        void setCameraPosition(const alg::vector3f& pos);
        void setCameraAngles(float yaw, float pitch, float roll);

        float* getCmrMatrixPointer();

    private:
        alg::matrix4f cmrViewMatrix;
        alg::matrix4f cmrRotateMatrix;
        alg::matrix4f cmrOffsetMatrix;

        alg::vector3f cmrPosition;

        float cmrFov, cmrAspect, cmrNear, cmrFar;

        float cmrYaw, cmrPitch, cmrRoll;
};

#endif
