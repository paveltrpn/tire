

#pragma once

#include "algebra/vector3.h"
#include "geometry/bounding_volumes.h"

namespace tire {

struct Flycam final {
    Flycam() = default;

    Flycam( const algebra::vector3f &eye, const algebra::vector3f &target,
            const algebra::vector3f &up );

    Flycam( const Flycam &other ) = delete;
    Flycam( Flycam &&other ) = delete;

    Flycam &operator=( const Flycam &other ) = delete;
    Flycam &operator=( Flycam &&other ) = delete;

    void setFov( float fov );
    void setAspect( float aspect );
    void setNcp( float ncp );
    void setFcp( float fcp );

private:
    float fov_{};
    float aspect_{};
    float ncp_{};
    float fcp_{};

    algebra::vector3f eye_{ 0.0f, 0.0f, 0.0f };
    // Default look vector +Z
    algebra::vector3f target_{ 0.0f, 0.0f, 1.0f };
    // Default left vector -X
    algebra::vector3f left_{ -1.0f, 0.0f, 0.0f };
    // One and only up vector +Y
    const algebra::vector3f up_{ 0.0f, 1.0f, 0.0f };

    float yaw_{};
    float pitch_{};
    float roll_{};

    BoundingSphere<float> bounding_{};
    float mass_{};
    algebra::vector3f velocity_{};
};

}  // namespace tire