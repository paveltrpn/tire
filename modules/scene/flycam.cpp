
#include <iostream>

#include "flycam.h"
#include <print>
#include "algebra/quaternion.h"
#include "algebra/vector3.h"

namespace tire {

Flycam::Flycam( const algebra::vector3<value_type> &eye, value_type azimuth,
                value_type elevation ) {
    azimuth_ = azimuth;
    elevation_ = elevation;
    roll_ = 0.0;

    eye_ = eye;
}

void Flycam::rotate( value_type azimuthOffset, value_type elevayionOffset ) {
    azimuth_ += azimuthOffset;

    // Clamp azimuth
    if ( ( azimuth_ > 360.0 ) || ( azimuth_ < -360.0 ) ) azimuth_ = 0.0;

    elevation_ += elevayionOffset;

    // Check elevation angles bound
#define ELEVATION_BOUND 80.0
    if ( elevation_ > ELEVATION_BOUND ) elevation_ = ELEVATION_BOUND;
    if ( elevation_ < -ELEVATION_BOUND ) elevation_ = -ELEVATION_BOUND;
}

void Flycam::setPosition( const algebra::vector3<value_type> &pos ) {
    eye_ = pos;
}

void Flycam::setAngles( value_type azimuth, value_type elevation,
                        value_type roll ) {
    azimuth_ = azimuth;
    elevation_ = elevation;
    roll_ = roll;
}

void Flycam::setFov( value_type fov ) {
    fov_ = fov;
}

void Flycam::setAspect( value_type aspect ) {
    aspect_ = aspect;
}

void Flycam::setNcp( value_type ncp ) {
    ncp_ = ncp;
}

void Flycam::setFcp( value_type fcp ) {
    fcp_ = fcp;
}

void Flycam::setMoveBit( FlycamMoveBits bit ) {
    moveMask_ |= ( (uint32_t)1 << bit );
}

void Flycam::unsetMoveBit( FlycamMoveBits bit ) {
    moveMask_ &= ~( (uint32_t)1 << bit );
}

void Flycam::unsetMoveAll() {
    moveMask_ = 0;
}

void Flycam::traverse() {
    if ( ( moveMask_ >> FlycamMoveBits::FORWARD ) & (uint32_t)1 )
        velocity_ += look_;

    if ( ( moveMask_ >> FlycamMoveBits::BACKWARD ) & (uint32_t)1 )
        velocity_ += look_.inverse();

    if ( ( moveMask_ >> FlycamMoveBits::RIGHT ) & (uint32_t)1 )
        velocity_ += right_.inverse();

    if ( ( moveMask_ >> FlycamMoveBits::LEFT ) & (uint32_t)1 )
        velocity_ += right_;

    eye_.plus( velocity_.scale( 0.5 ) );

    velocity_ = vector3<value_type>( 0.0, 0.0, 0.0 );
}

}  // namespace tire