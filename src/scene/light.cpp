
module;

export module scene:light;

import algebra;

namespace tire {

using namespace algebra;

enum kLightComponents {
    //
    CONSTANT = 0,
    LINEAR = 1,
    QUADRATIC = 2,
};

// Data type represents omni directional light. This type
// pass streight to shader and must have appropriate alignment.
export struct OmniLight final {
    using value_type = float;

    [[nodiscard]]
    auto position() const -> vector3<value_type> {
        return {
          position_.x(),
          position_.y(),
          position_.z(),
        };
    };

    [[nodiscard]]
    auto constant() -> value_type {
        return saturationComponents_[kLightComponents::CONSTANT];
    };

    [[nodiscard]]
    auto linear() -> value_type {
        return saturationComponents_[kLightComponents::LINEAR];
    };

    [[nodiscard]]
    auto quadratic() -> value_type {
        return saturationComponents_[kLightComponents::QUADRATIC];
    };

    [[nodiscard]]
    auto ambient() -> vector3<value_type> {
        return {
          ambient_.x(),
          ambient_.y(),
          ambient_.z(),
        };
    };

    [[nodiscard]]
    auto diffuse() -> vector3<value_type> {
        return {
          diffuse_.x(),
          diffuse_.y(),
          diffuse_.z(),
        };
    };

    [[nodiscard]]
    auto specular() -> vector3<value_type> {
        return {
          specular_.x(),
          specular_.y(),
          specular_.z(),
        };
    };

    [[maybe_unused]]
    auto setPosition( const vector3<value_type> &value ) -> OmniLight & {
        position_ = value;
        return *this;
    };

    [[maybe_unused]]
    auto setConstant( value_type value ) -> OmniLight & {
        saturationComponents_[kLightComponents::CONSTANT] = value;
        return *this;
    };

    [[maybe_unused]]
    auto setLinear( value_type value ) -> OmniLight & {
        saturationComponents_[kLightComponents::LINEAR] = value;
        return *this;
    };

    [[maybe_unused]]
    auto setQuadratic( value_type value ) -> OmniLight & {
        saturationComponents_[kLightComponents::QUADRATIC] = value;
        return *this;
    };

    [[maybe_unused]]
    auto setAmbient( const vector3<value_type> &value ) -> OmniLight & {
        ambient_ = value;
        return *this;
    };

    [[maybe_unused]]
    auto setDiffuse( const vector3<value_type> &value ) -> OmniLight & {
        diffuse_ = value;
        return *this;
    };

    [[maybe_unused]]
    auto setSpecular( const vector3<value_type> &value ) -> OmniLight & {
        specular_ = value;
        return *this;
    };

private:
    // All data is vec4 to preserve GLSL compatible alignment.

    vector4<value_type> position_{ 0.0, 0.0, 0.0, 0.0 };

    // Arrange as [CONSTANT, LINEAR, QUADRATIC]
    vector4<value_type> saturationComponents_{ 1.0, 1.0, 1.0, 0.0 };

    vector4<value_type> ambient_{ 1.0, 1.0, 1.0, 0.0 };
    vector4<value_type> diffuse_{ 1.0, 1.0, 1.0, 0.0 };
    vector4<value_type> specular_{ 1.0, 1.0, 1.0, 0.0 };
};

}  // namespace tire
