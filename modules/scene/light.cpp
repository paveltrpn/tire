
module;

export module scene:light;

import algebra;

namespace tire {

using namespace algebra;

export struct OmniLight final {
    using value_type = float;

    [[nodiscard]]
    auto position() const -> vector3<value_type> {
        return position_;
    };

    [[nodiscard]]
    auto constant() -> value_type {
        return constant_;
    };

    [[nodiscard]]
    auto linear() -> value_type {
        return linear_;
    };

    [[nodiscard]]
    auto quadratic() -> value_type {
        return quadratic_;
    };

    [[nodiscard]]
    auto ambient() -> vector3<value_type> {
        return ambient_;
    };

    [[nodiscard]]
    auto diffuse() -> vector3<value_type> {
        return diffuse_;
    };

    [[nodiscard]]
    auto specular() -> vector3<value_type> {
        return specular_;
    };

    [[maybe_unused]]
    auto setPosition( const vector3<value_type> &value ) -> OmniLight & {
        position_ = value;
        return *this;
    };

    [[maybe_unused]]
    auto setConstant( value_type value ) -> OmniLight & {
        constant_ = value;
        return *this;
    };

    [[maybe_unused]]
    auto setLinear( value_type value ) -> OmniLight & {
        linear_ = value;
        return *this;
    };

    [[maybe_unused]]
    auto setQuadratic( value_type value ) -> OmniLight & {
        quadratic_ = value;
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

    vector3<value_type> position_{ 0.0, 0.0, 0.0 };

    value_type constant_{ 1.0 };
    value_type linear_{ 1.0 };
    value_type quadratic_{ 1.0 };

    // NOTE: remove alignment!!!!!!
    char p1[8]{};

    vector3<value_type> ambient_{ 1.0, 1.0, 1.0 };

    char p2[4]{};

    vector3<value_type> diffuse_{ 1.0, 1.0, 1.0 };

    char p3[4]{};

    vector3<value_type> specular_{ 1.0, 1.0, 1.0 };

    char p4[4]{};
};

}  // namespace tire
