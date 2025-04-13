

#include "flycam.h"
#include <print>
#include "algebra/quaternion.h"
#include "algebra/vector3.h"

namespace tire {

Flycam::Flycam( const algebra::vector3f &eye, float azimuth, float elevation ) {
    azimuth_ = azimuth;
    elevation_ = elevation;
    roll_ = 0.0;

    eye_ = eye;
}

void Flycam::moveForward() {
    eye_.plus( look_.scale( 0.2 ) );
}

void Flycam::moveBackward() {
    eye_.minus( look_.scale( 0.2 ) );
}

void Flycam::strafeLeft() {
    eye_.plus( right_.scale( 0.2 ) );
}

void Flycam::strafeRight() {
    eye_.minus( right_.scale( 0.2 ) );
}

void Flycam::rotate( float azimuthOffset, float elevayionOffset ) {
    azimuth_ += azimuthOffset;

    // Clamp azimuth
    if ( ( azimuth_ > 360.0 ) || ( azimuth_ < -360.0 ) ) azimuth_ = 0.0;

    elevation_ += elevayionOffset;

    // Check elevation angles bound
#define ELEVATION_BOUND 80.0
    if ( elevation_ > ELEVATION_BOUND ) elevation_ = ELEVATION_BOUND;
    if ( elevation_ < -ELEVATION_BOUND ) elevation_ = -ELEVATION_BOUND;
}

void Flycam::setPosition( const algebra::vector3f &pos ) {
    eye_ = pos;
}

void Flycam::setAngles( float azimuth, float elevation, float roll ) {
    azimuth_ = azimuth;
    elevation_ = elevation;
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