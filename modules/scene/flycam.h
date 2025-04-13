#pragma once

#include <type_traits>
#include "algebra/vector3.h"
#include "algebra/matrix4.h"

#include "algebra/vector3.h"
#include "geometry/bounding_volumes.h"

#include "render_tag.h"

namespace tire {

struct Flycam final {
    Flycam() = default;

    Flycam( const algebra::vector3f &eye, float azimuth, float elevation );

    Flycam( const Flycam &other ) = delete;
    Flycam( Flycam &&other ) = delete;

    Flycam &operator=( const Flycam &other ) = delete;
    Flycam &operator=( Flycam &&other ) = delete;

    void setFov( float fov );
    void setAspect( float aspect );
    void setNcp( float ncp );
    void setFcp( float fcp );

    void moveForward();
    void moveBackward();
    void strafeLeft();
    void strafeRight();

    void rotate( float azimuthffset, float elevayionOffset );
    void setPosition( const algebra::vector3f &pos );
    void setAngles( float azimuth, float elevation, float roll );

    template <RenderType Type>
    algebra::matrix4f matrix() {
        algebra::matrix4f projection;
        // Choose projection matrix according to render type
        if constexpr ( std::is_same_v<Type, OpenGLTag> ) {
            projection =
                algebra::perspective<float>( fov_, aspect_, ncp_, fcp_ );
        } else if constexpr ( std::is_same_v<Type, VulkanTag> ) {
            projection =
                algebra::vperspective<float>( fov_, aspect_, ncp_, fcp_ );
        }

        // Get azimuth rotation matrix
        auto ar = algebra::rotate<float>( zenith_, azimuth_ );

        // Get actual "right side" direction vector
        right_ = ar.mult_vector3( { 1.0f, 0.0f, 0.0f } );

        // Get elevation rotation matrix
        auto er = algebra::rotate<float>( right_, elevation_ );

        // Get actual look direction vector
        look_ = er.mult_vector3( right_.cross( zenith_ ) );

        // Add roll rotation
        //

        // Combine both rotations
        auto rotation = er * ar;

        auto offset = algebra::translate<float>( eye_ );
        offset.transposeSelf();

        return offset * rotation * projection;
    }

    algebra::vector3f position() { return eye_; };
    float azimuth() { return azimuth_; };
    float elevation() { return elevation_; };
    float roll() { return roll_; };

private:
    // Perspective parameters
    float fov_{};
    float aspect_{};
    float ncp_{};
    float fcp_{};

    // Eye position
    algebra::vector3f eye_{ 0.0f, 0.0f, 0.0f };

    //
    algebra::vector3f zenith_{ 0.0f, 1.0f, 0.0f };
    algebra::vector3f right_{ 1.0f, 0.0f, 0.0f };
    algebra::vector3f look_{ 0.0f, 0.0f, 1.0f };

    //
    float azimuth_{};
    float elevation_{};
    float roll_{};

    // Bounding and mass
    BoundingSphere<float> bounding_{};
    float mass_{};
    algebra::vector3f velocity_{};
};

}  // namespace tire