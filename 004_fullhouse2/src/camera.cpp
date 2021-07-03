
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

    mtrx4 orientationMatrix = mtrx4FromEuler(cmrYaw, cmrPitch, cmrRoll);

    cmrViewMatrix = defaultViewMatrix  * orientationMatrix * offsetMatrix;
}