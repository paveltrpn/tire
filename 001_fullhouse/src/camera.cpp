
#include "camera.h"

void Camera::updateRotation() {
    cmrViewMatrix = cmrViewMatrix * cmrRotateMatrix;
}

void Camera::updatePosition() {
    cmrViewMatrix = cmrViewMatrix * cmrOffsetMatrix;
}

void Camera::updateViewParameters() {
    cmrViewMatrix = mtrx4FromPerspective(cmrFov, cmrAspect, cmrNear, cmrFar);
}

void Camera::setViewParameters(float fov, float aspect, float near, float far) {
    cmrFov = fov;
    cmrAspect = aspect;
    cmrNear = near;
    cmrFar = far;
}

void Camera::setCameraPosition(const vec3 &pos) {
    cmrPosition = pos;

    cmrOffsetMatrix = mtrx4FromOffset(cmrPosition);
    cmrOffsetMatrix.transposeSelf();
}

void Camera::setCameraAngles(float yaw, float pitch, float roll) {
    cmrYaw = yaw;
    cmrPitch = pitch;
    cmrRoll = roll;

    cmrRotateMatrix = mtrx4FromEuler(cmrYaw, cmrPitch, cmrRoll);
}

float* Camera::getCmrMatrixPointer() {
    return cmrViewMatrix.getData();
}
