#pragma once

#include <string>
#include <cstdint>

#include "algebra/vector3.h"
#include "algebra/matrix4.h"

#include "algebra/vector3.h"
#include "geometry/bounding_volumes.h"

namespace tire {

// enum FlycamMoveBits {
// NONE = 1 << 0,
// FORWARD = 1 << 1,
// BACKWARD = 1 << 2,
// LEFT = 1 << 3,
// RIGHT = 1 << 4,
// UP = 1 << 5,
// DOWN = 1 << 6,
// };

enum FlycamMoveBits {
    NONE = 1,
    FORWARD = 2,
    BACKWARD = 3,
    LEFT = 4,
    RIGHT = 5,
    UP = 6,
    DOWN = 7,
};

struct Flycam final {
    // NOTE: 64-bit prescision float!
    using value_type = double;

    Flycam() = default;

    Flycam( const algebra::vector3d& eye, value_type azimuth,
            value_type elevation );

    Flycam( const Flycam& other ) = delete;
    Flycam( Flycam&& other ) = delete;

    auto operator=( const Flycam& other ) -> Flycam& = delete;
    auto operator=( Flycam&& other ) -> Flycam& = delete;

    [[maybe_unused]]
    Flycam& setFov( value_type fov ) {
        fov_ = fov;
        return *this;
    }

    [[maybe_unused]]
    Flycam& setAspect( value_type aspect ) {
        aspect_ = aspect;
        return *this;
    }

    [[maybe_unused]]
    Flycam& setNcp( value_type ncp ) {
        ncp_ = ncp;
        return *this;
    }

    [[maybe_unused]]
    Flycam& setFcp( value_type fcp ) {
        fcp_ = fcp;
        return *this;
    }

    [[maybe_unused]]
    Flycam& setName( const std::string& value ) {
        name_ = value;
        return *this;
    };

    void setMoveBit( FlycamMoveBits bit );
    void unsetMoveBit( FlycamMoveBits bit );
    void unsetMoveAll();

    void rotate( value_type azimuthffset, value_type elevayionOffset );
    void setPosition( const algebra::vector3d& pos );
    void setAngles( value_type azimuth, value_type elevation, value_type roll );

    auto matrix() -> algebra::matrix4<value_type> {
        algebra::matrix4<value_type> projection;
        projection =
            algebra::perspective<value_type>( fov_, aspect_, ncp_, fcp_ );
        // projection =
        // algebra::vperspective<value_type>( fov_, aspect_, ncp_, fcp_ );

        // Get azimuth rotation matrix
        const auto& ar = algebra::rotate<value_type>( zenith_, azimuth_ );

        // Get actual "right side" direction vector
        right_ = ar.mult_vector3( { 1.0f, 0.0f, 0.0f } );

        // Get elevation rotation matrix
        const auto& er = algebra::rotate<value_type>( right_, elevation_ );

        // Get actual look direction vector
        look_ = er.mult_vector3( right_.cross( zenith_ ) );

        // Add roll rotation
        //

        // Get offset matrix
        auto&& offset = algebra::translate<value_type>( eye_ );
        offset.transposeSelf();

        return offset * er * ar * projection;
    }

    [[nodiscard]]
    algebra::vector3<value_type> position() const {
        return eye_;
    };

    [[nodiscard]]
    value_type azimuth() const {
        return azimuth_;
    };

    [[nodiscard]]
    value_type elevation() const {
        return elevation_;
    };

    [[nodiscard]]
    value_type roll() const {
        return roll_;
    };

    [[nodiscard]]
    std::string name() const {
        return name_;
    };

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