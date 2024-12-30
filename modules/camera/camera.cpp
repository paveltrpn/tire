
#include "camera.h"

namespace tire {

void Camera::setDirty() {
    dirty_ = true;
};

void Camera::unSetDirty() {
    dirty_ = false;
}

// recalculate projection matrix despite on _dirty flag
algebra::matrix4f Camera::forceGetMatrix() {
    update();
    unSetDirty();
    return matrix_;
}

algebra::matrix4f Camera::getMatrix() {
    if ( !dirty_ ) {
        return matrix_;
    } else {
        return forceGetMatrix();
    }
}

void Camera::move( const algebra::vector3f &offset ) {
    position_.plus( offset );
    setDirty();
}

void Camera::rotate( float yawOffset, float pitchOffset, float rollOffset ) {
    yaw_ += yawOffset;
    pitch_ += pitchOffset;
    roll_ += rollOffset;
    setDirty();
}

void Camera::setPosition( const algebra::vector3f &pos ) {
    position_ = pos;
    setDirty();
}

void Camera::setAngles( float yaw, float pitch, float roll ) {
    yaw_ = yaw;
    pitch_ = pitch;
    roll_ = roll;
    setDirty();
}
Perspective::Perspective( float fov, float aspect, float ncp, float fcp )
    : fov_{ fov }
    , aspect_{ aspect }
    , ncp_{ ncp }
    , fcp_{ fcp } {
    update();
    unSetDirty();
}

void Perspective::setFov( float fov ) {
    fov_ = fov;
    setDirty();
}

void Perspective::setAspect( float aspect ) {
    aspect_ = aspect;
    setDirty();
}

void Perspective::setNcp( float ncp ) {
    ncp_ = ncp;
    setDirty();
}

void Perspective::setFcp( float fcp ) {
    fcp_ = fcp;
    setDirty();
}

void Perspective::update() {
    auto projection = algebra::perspective<float>( fov_, aspect_, ncp_, fcp_ );
    auto offset = algebra::translate( position_ );
    auto rotation = algebra::rotate( yaw_, pitch_, roll_ );

    matrix_ = algebra::transpose( offset ) * rotation * projection;
}

Orthographic::Orthographic( float left, float right, float bottom, float top,
                            float near, float far )
    : left_{ left }
    , right_{ right }
    , bottom_{ bottom }
    , top_{ top }
    , near_{ near }
    , far_{ far } {
    update();
    unSetDirty();
}

void Orthographic::update() {
    auto projection =
        algebra::orthographic( left_, right_, bottom_, top_, near_, far_ );
    auto offset = algebra::translate( position_ );
    auto rotation = algebra::rotate( yaw_, pitch_, roll_ );

    matrix_ = projection * rotation * algebra::transpose( offset );
}

}  // namespace tire
