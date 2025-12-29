
module;

#include <string>
#include <cstdint>

export module scene:flycam;

import algebra;
import geometry;

namespace tire {

using namespace algebra;

// enum FlycamMoveBits {
// NONE = 1 << 0,
// FORWARD = 1 << 1,
// BACKWARD = 1 << 2,
// LEFT = 1 << 3,
// RIGHT = 1 << 4,
// UP = 1 << 5,
// DOWN = 1 << 6,
// };

export enum FlycamMoveBits {
    NONE = 1,
    FORWARD = 2,
    BACKWARD = 3,
    LEFT = 4,
    RIGHT = 5,
    UP = 6,
    DOWN = 7,
};

export struct Flycam final {
    // NOTE: 64-bit prescision float!
    using value_type = double;

    Flycam() = default;

    Flycam( const algebra::vector3<value_type> &eye, value_type azimuth, value_type elevation ) {
        azimuth_ = azimuth;
        elevation_ = elevation;
        roll_ = 0.0;

        eye_ = eye;
    }

    Flycam( const Flycam &other ) = delete;
    Flycam( Flycam &&other ) = delete;
    auto operator=( const Flycam &other ) -> Flycam & = delete;
    auto operator=( Flycam &&other ) -> Flycam & = delete;

    ~Flycam() = default;

    auto setFov( value_type fov ) -> Flycam & {
        //
        fov_ = fov;
        return *this;
    }

    auto setAspect( value_type aspect ) -> Flycam & {
        //
        aspect_ = aspect;
        return *this;
    }

    auto setNcp( value_type ncp ) -> Flycam & {
        //
        ncp_ = ncp;
        return *this;
    }

    auto setFcp( value_type fcp ) -> Flycam & {
        //
        fcp_ = fcp;
        return *this;
    }

    auto setName( const std::string &value ) -> Flycam & {
        //
        name_ = value;
        return *this;
    };

    void setMoveBit( FlycamMoveBits bit ) { moveMask_ |= ( (uint32_t)1 << bit ); }

    void unsetMoveBit( FlycamMoveBits bit ) { moveMask_ &= ~( (uint32_t)1 << bit ); }

    void unsetMoveAll() { moveMask_ = 0; }

    void rotate( value_type azimuthOffset, value_type elevayionOffset ) {
        azimuth_ += azimuthOffset;

        // Clamp azimuth
        if ( ( azimuth_ > 360.0 ) || ( azimuth_ < -360.0 ) ) azimuth_ = 0.0;

        elevation_ += elevayionOffset;

        // Check elevation angles bound
#define ELEVATION_BOUND 80.0
        if ( elevation_ > ELEVATION_BOUND ) elevation_ = ELEVATION_BOUND;
        if ( elevation_ < -ELEVATION_BOUND ) elevation_ = -ELEVATION_BOUND;
    }

    void setPosition( const algebra::vector3<value_type> &pos ) { eye_ = pos; }

    void setAngles( value_type azimuth, value_type elevation, value_type roll ) {
        azimuth_ = azimuth;
        elevation_ = elevation;
        roll_ = roll;
    }
    auto matrix() -> algebra::matrix4<value_type> {
        algebra::matrix4<value_type> projection;
        projection = algebra::perspective<value_type>( fov_, aspect_, ncp_, fcp_ );
        // projection =
        // algebra::vperspective<value_type>( fov_, aspect_, ncp_, fcp_ );

        // Get azimuth rotation matrix
        const auto &ar = algebra::rotate<value_type>( zenith_, azimuth_ );

        // Get actual "right side" direction vector
        right_ = ar.mult_vector3( { 1.0f, 0.0f, 0.0f } );

        // Get elevation rotation matrix
        const auto &er = algebra::rotate<value_type>( right_, elevation_ );

        // Get actual look direction vector
        look_ = er.mult_vector3( right_.cross( zenith_ ) );

        // Add roll rotation
        //

        // Get offset matrix
        auto &&offset = algebra::translate<value_type>( eye_ );
        offset.transpose_self();

        return offset * er * ar * projection;
    }

    [[nodiscard]]
    auto position() const -> algebra::vector3<value_type> {
        return eye_;
    };

    [[nodiscard]]
    auto azimuth() const -> value_type {
        return azimuth_;
    };

    [[nodiscard]]
    auto elevation() const -> value_type {
        return elevation_;
    };

    [[nodiscard]]
    auto roll() const -> value_type {
        return roll_;
    };

    [[nodiscard]]
    auto name() const -> std::string {
        return name_;
    };

    void traverse() {
        if ( ( moveMask_ >> FlycamMoveBits::FORWARD ) & (uint32_t)1 ) velocity_ += look_;

        if ( ( moveMask_ >> FlycamMoveBits::BACKWARD ) & (uint32_t)1 ) velocity_ += look_.inverse();

        if ( ( moveMask_ >> FlycamMoveBits::RIGHT ) & (uint32_t)1 ) velocity_ += right_.inverse();

        if ( ( moveMask_ >> FlycamMoveBits::LEFT ) & (uint32_t)1 ) velocity_ += right_;

        eye_.plus( velocity_.scale( 0.5 ) );

        velocity_ = vector3<value_type>( 0.0, 0.0, 0.0 );
    }

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
    BoundingSphere bounding_{};
    value_type mass_{};
    algebra::vector3<value_type> velocity_{};

    std::string name_{};
};

}  // namespace tire
