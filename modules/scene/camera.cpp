
#include "camera.h"
#include "algebra/matrix4.h"

namespace tire {

void Camera::move( const algebra::vector3f &offset ) {
    position_.plus( offset );
}

void Camera::rotate( float yawOffset, float pitchOffset, float rollOffset ) {
    yaw_ += yawOffset;
    pitch_ += pitchOffset;
    roll_ += rollOffset;
}

void Camera::setPosition( const algebra::vector3f &pos ) {
    position_ = pos;
}

void Camera::setAngles( float yaw, float pitch, float roll ) {
    yaw_ = yaw;
    pitch_ = pitch;
    roll_ = roll;
}

Perspective::Perspective( float fov, float aspect, float ncp, float fcp )
    : fov_{ fov }
    , aspect_{ aspect }
    , ncp_{ ncp }
    , fcp_{ fcp } {
}

void Perspective::setFov( float fov ) {
    fov_ = fov;
}

void Perspective::setAspect( float aspect ) {
    aspect_ = aspect;
}

void Perspective::setNcp( float ncp ) {
    ncp_ = ncp;
}

void Perspective::setFcp( float fcp ) {
    fcp_ = fcp;
}

}  // namespace tire
