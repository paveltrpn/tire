
#include "camera.h"

namespace tire {

algebra::matrix4f Camera::matrix() {
    update();
    return matrix_;
}

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
    update();
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

void Perspective::update() {
    auto projection = algebra::perspective<float>( fov_, aspect_, ncp_, fcp_ );
    auto offset = algebra::translate( position_ );
    auto rotation = algebra::rotate( yaw_, pitch_, roll_ );

    offset.transposeSelf();
    matrix_ = offset * rotation * projection;
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
}

void Orthographic::update() {
    auto projection =
        algebra::orthographic( left_, right_, bottom_, top_, near_, far_ );
    auto offset = algebra::translate( position_ );
    auto rotation = algebra::rotate( yaw_, pitch_, roll_ );

    offset.transposeSelf();
    matrix_ = projection * rotation * offset;
}

}  // namespace tire
