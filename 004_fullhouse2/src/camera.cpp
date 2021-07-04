
#include "camera.h"

void CPerspCamera::setCameraAngles(float yaw, float pitch, float roll) {
    cmrYaw      = yaw;
    cmrPitch    = pitch;
    cmrRoll     = roll;
}

void CPerspCamera::setCameraPosition(const vec3 &pos) {
    cmrPosition = pos;
}

void CPerspCamera::moveCamera(const vec3 &offset) {
    cmrPosition = vec3Sum(cmrPosition, offset); 
}

void CPerspCamera::rotateCamera(float yaw, float pitch, float roll) {
    cmrYaw      += yaw;
    cmrPitch    += pitch;
    cmrRoll     += roll;
}

float* CPerspCamera::getCmrMatrixPointer() {
    return cmrViewMatrix.getData();
}

void CPerspCamera::setViewParameters(float fov, float aspect, float near, float far) {
    cmrFov = fov;
    cmrAspect = aspect;
    cmrNear = near;
    cmrFar = far;
}

void CPerspCamera::updateViewMatrix() {
    mtrx4 defaultViewMatrix = mtrx4FromPerspective(cmrFov, cmrAspect, cmrNear, cmrFar);
    
    mtrx4 offsetMatrix = mtrx4FromOffset(cmrPosition);
    offsetMatrix.transposeSelf();

    mtrx4 orientationYaw   = mtrx4FromRtnYaw(cmrYaw);
    mtrx4 orientationPitch = mtrx4FromRtnPitch(cmrPitch);
    mtrx4 orientationRoll  = mtrx4FromRtnRoll(cmrRoll);
    
    mtrx4 orientationMatrix = orientationYaw * orientationPitch * orientationRoll;
    
    cmrViewMatrix = defaultViewMatrix * orientationMatrix * offsetMatrix;
}

//===================================================================================

void CPerspLookAtCamera::calculateMoveVectors() {
    vec3 eyeDir = vec3Sub(cmrEye, cmrTarget);
    
    cmrLeftVec = vec3Cross(cmrUp, eyeDir);
    cmrLeftVec.normalize();

    cmrForwardVec = vec3Cross(cmrUp, cmrLeftVec);
    cmrForwardVec.normalize();
}

void CPerspLookAtCamera::setViewParameters(float fov, float aspect, float near, float far) {
    cmrFov = fov;
    cmrAspect = aspect;
    cmrNear = near;
    cmrFar = far;
}

void CPerspLookAtCamera::updateViewMatrix() { 
    mtrx4 perspMatrix = mtrx4FromPerspective(cmrFov, cmrAspect, cmrNear, cmrFar);
    mtrx4 lookAtMatrix = mtrx4FromLookAt(cmrEye, cmrTarget, cmrUp);

    cmrViewMatrix = perspMatrix * lookAtMatrix;
}

void CPerspLookAtCamera::setLookPoints(vec3 eye, vec3 target) { 
    cmrEye = eye;
    cmrTarget = target;

    calculateMoveVectors();
}

void CPerspLookAtCamera::setUpVec(vec3 up) {
    cmrUp = up;

    calculateMoveVectors();
};

void CPerspLookAtCamera::moveViewPointsSideway(float spd) {
    cmrTarget = vec3Sum(cmrTarget, {cmrLeftVec[0]*spd, cmrLeftVec[1]*spd, cmrLeftVec[2]*spd});
    cmrEye    = vec3Sum(cmrEye,    {cmrLeftVec[0]*spd, cmrLeftVec[1]*spd, cmrLeftVec[2]*spd});
};

void CPerspLookAtCamera::moveViewPointsForward(float spd) {
    cmrTarget = vec3Sum(cmrTarget, {cmrForwardVec[0]*spd, cmrForwardVec[1]*spd, cmrForwardVec[2]*spd});
    cmrEye    = vec3Sum(cmrEye,    {cmrForwardVec[0]*spd, cmrForwardVec[1]*spd, cmrForwardVec[2]*spd});
};


float* CPerspLookAtCamera::getCmrMatrixPointer() {
    return cmrViewMatrix.getData();
};
