
#ifndef __camera_h_
#define __camera_h_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class CPerspCamera {
    public:
        CPerspCamera() {
            cmrFov = 45.0f;
            cmrAspect = 4.0f / 3.0f;
            cmrNear = 0.1;
            cmrFar = 100.0f;

            cmrYaw = cmrPitch = cmrRoll = 0.0f;

            cmrPosition = glm::vec3(0.0f);

            cmrViewMatrix = glm::perspective(cmrFov, cmrAspect, cmrNear, cmrFar);
        };

        ~CPerspCamera() {};

        void    setViewParameters(float fov, float aspect, float near, float far);
        void    updateViewMatrix();
        float*  getCmrMatrixPointer();

        void    setCameraAngles(float yaw, float pitch, float roll);
        void    setCameraPosition(const glm::vec3 &pos);

        void    moveCamera(const glm::vec3 &offset);
        void    rotateCamera(float yaw, float pitch, float roll);

    private:
        glm::mat4 cmrViewMatrix;

        float cmrFov, cmrAspect, cmrNear, cmrFar;
        glm::vec3  cmrPosition;
        float cmrYaw, cmrPitch, cmrRoll;
};

class CPerspLookAtCamera {
    public:
        CPerspLookAtCamera() {
            cmrFov = 45.0f;
            cmrAspect = 4.0f / 3.0f;
            cmrNear = 0.1;
            cmrFar = 100.0f;

            cmrEye = glm::vec3(0.0f, 0.0f, 5.0f);
            cmrTarget = glm::vec3(0.0f, 0.0f, 0.0f);

            cmrUp = glm::vec3(0.0f, 1.0f, 0.0f);

            calculateMoveVectors();

            glm::mat4 perspMatrix = glm::perspective(cmrFov, cmrAspect, cmrNear, cmrFar);
            glm::mat4 lookAtMatrix = glm::lookAt(cmrEye, cmrTarget, cmrUp);

            cmrViewMatrix = perspMatrix * lookAtMatrix;
        };

        ~CPerspLookAtCamera() {};

        void    calculateMoveVectors();

        void    setViewParameters(float fov, float aspect, float near, float far);
        void    updateViewMatrix();
        void    setLookPoints(glm::vec3 eye, glm::vec3 target);
        void    setUpVec(const glm::vec3& up);
        void    moveViewPointsSideway(float spd);
        void    moveViewPointsForward(float spd);
        void    rotateEyeUp(float angl);
        
        float*  getCmrMatrixPointer();

    private:
        glm::mat4 cmrViewMatrix;

        glm::vec3 cmrEye, cmrTarget, cmrUp;
        glm::vec3 cmrForwardVec, cmrLeftVec;

        float cmrFov, cmrAspect, cmrNear, cmrFar;
};

#endif