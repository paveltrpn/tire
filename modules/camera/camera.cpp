
#include "camera.h"

namespace tire::camera
{

void Camera::setDirty() {
    dirty_ = true;
};

void Camera::unSetDirty() {
    dirty_ = false;
}

// recalculate projection matrix despite on _dirty flag
algebra::matrix4d Camera::forceGetMatrix() {
    update();
    unSetDirty();
    return matrix_;
}

algebra::matrix4d Camera::getMatrix() {
    if ( !dirty_ ) {
        return matrix_;
    } else {
        return forceGetMatrix();
    }
}

void Camera::move( const algebra::vector3d &offset ) {
    position_.plus( offset );
    setDirty();
}

void Camera::rotate( double yawOffset, double pitchOffset, double rollOffset ) {
    yaw_ += yawOffset;
    pitch_ += pitchOffset;
    roll_ += rollOffset;
    setDirty();
}

void Camera::setPosition( const algebra::vector3d &pos ) {
    position_ = pos;
    setDirty();
}

void Camera::setAngles( double yaw, double pitch, double roll ) {
    yaw_ = yaw;
    pitch_ = pitch;
    roll_ = roll;
    setDirty();
}
Perspective::Perspective( double fov, double aspect, double ncp, double fcp )
    : fov_{ fov }
    , aspect_{ aspect }
    , ncp_{ ncp }
    , fcp_{ fcp } {
    update();
    unSetDirty();
}

void Perspective::setFov( double fov ) {
    fov_ = fov;
    setDirty();
}

void Perspective::setAspect( double aspect ) {
    aspect_ = aspect;
    setDirty();
}

void Perspective::setNcp( double ncp ) {
    ncp_ = ncp;
    setDirty();
}

void Perspective::setFcp( double fcp ) {
    fcp_ = fcp;
    setDirty();
}

void Perspective::update() {
    auto projection = algebra::perspective<double>( fov_, aspect_, ncp_, fcp_ );
    auto offset = algebra::translate( position_ );
    auto rotation = algebra::rotate( yaw_, pitch_, roll_ );

    matrix_ = projection * rotation * algebra::transpose( offset );
}

Orthographic::Orthographic( double left, double right, double bottom, double top, double near, double far )
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
    auto projection = algebra::orthographic( left_, right_, bottom_, top_, near_, far_ );
    auto offset = algebra::translate( position_ );
    auto rotation = algebra::rotate( yaw_, pitch_, roll_ );

    matrix_ = projection * rotation * algebra::transpose( offset );
}

} // namespace tire::camera
