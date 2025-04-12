

#include "flycam.h"

namespace tire {

Flycam::Flycam( const algebra::vector3f &eye, const algebra::vector3f &target,
                const algebra::vector3f &up ) {
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