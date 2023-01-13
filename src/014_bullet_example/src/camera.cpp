
#include "camera.h"

void CPerspCamera::setCameraAngles(float yaw, float pitch, float roll) {
    cmrYaw      = yaw;
    cmrPitch    = pitch;
    cmrRoll     = roll;
}

void CPerspCamera::setCameraPosition(const glm::vec3 &pos) {
    cmrPosition = pos;
}

void CPerspCamera::moveCamera(const glm::vec3 &offset) {
    cmrPosition = cmrPosition + offset; 
}

void CPerspCamera::rotateCamera(float yaw, float pitch, float roll) {
    cmrYaw      += yaw;
    cmrPitch    += pitch;
    cmrRoll     += roll;
}

float* CPerspCamera::getCmrMatrixPointer() {
    return glm::value_ptr(cmrViewMatrix);
}

void CPerspCamera::setViewParameters(float fov, float aspect, float near, float far) {
    cmrFov = fov;
    cmrAspect = aspect;
    cmrNear = near;
    cmrFar = far;
}

constexpr float degToRad(float deg) {
	return deg * 3.1415f/180.0f;
}

void CPerspCamera::updateViewMatrix() {
    glm::mat4 defaultViewMatrix = glm::perspective(cmrFov, cmrAspect, cmrNear, cmrFar);
    
    glm::mat4 offsetMatrix = glm::translate(glm::mat4(1.0f), cmrPosition);
    // offsetMatrix = glm::transpose(offsetMatrix);
    
    glm::mat4 orientationMatrix = glm::eulerAngleYXZ((cmrYaw), degToRad(cmrPitch), degToRad(cmrRoll));
    
    cmrViewMatrix = defaultViewMatrix * offsetMatrix * orientationMatrix;
}

//===================================================================================

void CPerspLookAtCamera::calculateMoveVectors() {
    glm::vec3 eyeDir = cmrEye - cmrTarget;
    
    cmrLeftVec = glm::cross(cmrUp, eyeDir);
    cmrLeftVec = glm::normalize(cmrLeftVec);

    cmrForwardVec = glm::cross(cmrUp, cmrLeftVec);
    cmrForwardVec = glm::normalize(cmrForwardVec);
}

void CPerspLookAtCamera::setViewParameters(float fov, float aspect, float near, float far) {
    cmrFov = fov;
    cmrAspect = aspect;
    cmrNear = near;
    cmrFar = far;
}

void CPerspLookAtCamera::updateViewMatrix() { 
    glm::mat4 perspMatrix = glm::perspective(cmrFov, cmrAspect, cmrNear, cmrFar);
    glm::mat4 lookAtMatrix = glm::lookAt(cmrEye, cmrTarget, cmrUp);

    cmrViewMatrix = perspMatrix * lookAtMatrix;
}

void CPerspLookAtCamera::setLookPoints(glm::vec3 eye, glm::vec3 target) { 
    cmrEye = eye;
    cmrTarget = target;

    calculateMoveVectors();
}

void CPerspLookAtCamera::setUpVec(const glm::vec3& up) {
    cmrUp = up;

    calculateMoveVectors();
};

void CPerspLookAtCamera::moveViewPointsSideway(float spd) {
    cmrTarget = cmrTarget + glm::vec3(cmrLeftVec[0]*spd, cmrLeftVec[1]*spd, cmrLeftVec[2]*spd);
    cmrEye    = cmrEye + glm::vec3(cmrLeftVec[0]*spd, cmrLeftVec[1]*spd, cmrLeftVec[2]*spd);
};

void CPerspLookAtCamera::moveViewPointsForward(float spd) {
    cmrTarget = cmrTarget + glm::vec3(cmrForwardVec[0]*spd, cmrForwardVec[1]*spd, cmrForwardVec[2]*spd);
    cmrEye    = cmrEye + glm::vec3(cmrForwardVec[0]*spd, cmrForwardVec[1]*spd, cmrForwardVec[2]*spd);
};

void CPerspLookAtCamera::rotateEyeUp(float angl) {
    glm::mat4 rtnMatrix = glm::rotate(degToRad(angl), cmrUp);
    glm::vec3 defaultEye = cmrEye - cmrTarget;
    glm::vec3 newEye = glm::vec3(rtnMatrix * glm::vec4(defaultEye, 1.0f));

    cmrEye = newEye + cmrTarget;

    calculateMoveVectors();
}

float* CPerspLookAtCamera::getCmrMatrixPointer() {
    return glm::value_ptr(cmrViewMatrix);
};
