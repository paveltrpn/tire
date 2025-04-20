#pragma once

#include <string>
#include <type_traits>

#include "algebra/vector3.h"
#include "algebra/matrix4.h"

#include "algebra/vector3.h"
#include "geometry/bounding_volumes.h"

#include "render_tag.h"

namespace tire {

enum FlycamMoveBits {
    NONE = 1 << 0,
    FORWARD = 1 << 1,
    BACKWARD = 1 << 2,
    LEFT = 1 << 3,
    RIGHT = 1 << 4,
    UP = 1 << 5,
    DOWN = 1 << 6,
};

struct Flycam final {
    using value_type = double;

    Flycam() = default;

    Flycam( const algebra::vector3d &eye, value_type azimuth,
            value_type elevation );

    Flycam( const Flycam &other ) = delete;
    Flycam( Flycam &&other ) = delete;

    Flycam &operator=( const Flycam &other ) = delete;
    Flycam &operator=( Flycam &&other ) = delete;

    void setFov( value_type fov );
    void setAspect( value_type aspect );
    void setNcp( value_type ncp );
    void setFcp( value_type fcp );
    void setName( const std::string &value ) { name_ = value; };

    void setMoveBit( FlycamMoveBits bit );
    void unsetMoveBit( FlycamMoveBits bit );
    void unsetMoveAll();

    void rotate( value_type azimuthffset, value_type elevayionOffset );
    void setPosition( const algebra::vector3d &pos );
    void setAngles( value_type azimuth, value_type elevation, value_type roll );

    template <RenderType Type>
    algebra::matrix4<value_type> matrix() {
        algebra::matrix4<value_type> projection;
        // Choose projection matrix according to render type
        if constexpr ( std::is_same_v<Type, OpenGLTag> ) {
            projection =
                algebra::perspective<value_type>( fov_, aspect_, ncp_, fcp_ );
        } else if constexpr ( std::is_same_v<Type, VulkanTag> ) {
            projection =
                algebra::vperspective<value_type>( fov_, aspect_, ncp_, fcp_ );
        }

        // Get azimuth rotation matrix
        auto ar = algebra::rotate<value_type>( zenith_, azimuth_ );

        // Get actual "right side" direction vector
        right_ = ar.mult_vector3( { 1.0f, 0.0f, 0.0f } );

        // Get elevation rotation matrix
        auto er = algebra::rotate<value_type>( right_, elevation_ );

        // Get actual look direction vector
        look_ = er.mult_vector3( right_.cross( zenith_ ) );

        // Add roll rotation
        //

        // Combine both rotations
        auto rotation = er * ar;

        auto offset = algebra::translate<value_type>( eye_ );
        offset.transposeSelf();

        return offset * rotation * projection;
    }

    algebra::vector3<value_type> position() { return eye_; };
    value_type azimuth() { return azimuth_; };
    value_type elevation() { return elevation_; };
    value_type roll() { return roll_; };
    std::string name() { return name_; };

    void traverse();

private:
    // Perspective parameters
    value_type fov_{};
    value_type aspect_{};
    value_type ncp_{};
    value_type fcp_{};

    // Eye position
    algebra::vector3<value_type> eye_{ 0.0f, 0.0f, 0.0f };

    //
    algebra::vector3<value_type> zenith_{ 0.0f, 1.0f, 0.0f };
    algebra::vector3<value_type> right_{ 1.0f, 0.0f, 0.0f };
    algebra::vector3<value_type> look_{ 0.0f, 0.0f, 1.0f };

    //
    value_type azimuth_{};
    value_type elevation_{};
    value_type roll_{};

    uint32_t moveMask_{};

    // Bounding and mass
    BoundingSphere<value_type> bounding_{};
    value_type mass_{};
    algebra::vector3<value_type> velocity_{};

    std::string name_{};
};

}  // namespace tire