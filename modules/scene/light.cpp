
module;

export module scene:light;

import algebra;

namespace tire {

using namespace algebra;

export struct OmniLight final {
    using value_type = float;

    [[nodiscard]]
    vector3<value_type> position() {
        return position_;
    };

    [[nodiscard]]
    value_type constant() {
        return constant_;
    };

    [[nodiscard]]
    value_type linear() {
        return linear_;
    };

    [[nodiscard]]
    value_type quadratic() {
        return quadratic_;
    };

    [[nodiscard]]
    vector3<value_type> ambient() {
        return ambient_;
    };

    [[nodiscard]]
    vector3<value_type> diffuse() {
        return diffuse_;
    };

    [[nodiscard]]
    vector3<value_type> specular() {
        return specular_;
    };

    [[maybe_unused]]
    OmniLight &setPosition( const vector3<value_type> &value ) {
        position_ = value;
        return *this;
    };

    [[maybe_unused]]
    OmniLight &setConstant( value_type value ) {
        constant_ = value;
        return *this;
    };

    [[maybe_unused]]
    OmniLight &setLinear( value_type value ) {
        linear_ = value;
        return *this;
    };

    [[maybe_unused]]
    OmniLight &setQuadratic( value_type value ) {
        quadratic_ = value;
        return *this;
    };

    [[maybe_unused]]
    OmniLight &setAmbient( const vector3<value_type> &value ) {
        ambient_ = value;
        return *this;
    };

    [[maybe_unused]]
    OmniLight &setDiffuse( const vector3<value_type> &value ) {
        diffuse_ = value;
        return *this;
    };

    [[maybe_unused]]
    OmniLight &setSpecular( const vector3<value_type> &value ) {
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
