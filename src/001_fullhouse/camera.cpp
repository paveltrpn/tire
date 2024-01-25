
#include "camera.h"

import toy_std;

namespace alg = toy::algebra;
 
void Camera::updateRotation() {
    //cmrViewMatrix = cmrViewMatrix * cmrRotateMatrix;
}

void Camera::updatePosition() {
    cmrViewMatrix = cmrViewMatrix * cmrOffsetMatrix;
}

void Camera::updateViewParameters() {
    cmrViewMatrix.set_perspective(cmrFov, cmrAspect, cmrNear, cmrFar);
}

void Camera::setViewParameters(float fov, float aspect, float near, float far) {
    cmrFov = fov;
    cmrAspect = aspect;
    cmrNear = near;
    cmrFar = far;
}

void Camera::setCameraPosition(const alg::vector3f& pos) {
    cmrPosition = pos;

    cmrOffsetMatrix.set_offset(cmrPosition);
    cmrOffsetMatrix.transpose_self();
}

void Camera::setCameraAngles(float yaw, float pitch, float roll) {
    cmrYaw = yaw;
    cmrPitch = pitch;
    cmrRoll = roll;

    cmrRotateMatrix.set_euler(cmrYaw, cmrPitch, cmrRoll);
}

float* Camera::getCmrMatrixPointer() {
    return cmrViewMatrix.data();
}
