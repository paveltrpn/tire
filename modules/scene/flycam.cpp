

#include "flycam.h"
#include <print>
#include "algebra/quaternion.h"
#include "algebra/vector3.h"

namespace tire {

Flycam::Flycam( const algebra::vector3f &eye,
                const algebra::vector3f &target ) {
    // Got look vector
    auto look = target - eye;
    look.normalizeSelf();

    // Obtain quaternion that rotates between defalt look vector (+z)
    // and current look vector
    const auto quat =
        algebra::quaternionf{ look, algebra::vector3f{ 0.0, 0.0, 1.0 } };

    const auto [roll, pitch, yaw] = quat.toEuler();

    roll_ = roll;
    pitch_ = pitch;
    yaw_ = yaw;

    eye_ = eye;
}
Flycam::Flycam( const algebra::vector3f &eye, float yaw, float pitch,
                float roll ) {
    roll_ = roll;
    pitch_ = pitch;
    yaw_ = yaw;

    eye_ = eye;
}

void Flycam::move( const algebra::vector3f &offset ) {
    eye_.plus( offset );
}

void Flycam::rotate( float yawOffset, float pitchOffset, float rollOffset ) {
    yaw_ += yawOffset;
    pitch_ += pitchOffset;
    roll_ += rollOffset;
}

void Flycam::setPosition( const algebra::vector3f &pos ) {
    eye_ = pos;
}

void Flycam::setAngles( float yaw, float pitch, float roll ) {
    yaw_ = yaw;
    pitch_ = pitch;
    roll_ = roll;
}

void Flycam::setFov( float fov ) {
    fov_ = fov;
}

void Flycam::setAspect( float aspect ) {
    aspect_ = aspect;
}

void Flycam::setNcp( float ncp ) {
    ncp_ = ncp;
}

void Flycam::setFcp( float fcp ) {
    fcp_ = fcp;
}

}  // namespace tire