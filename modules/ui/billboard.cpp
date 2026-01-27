
module;

#include <array>

export module ui:billboard;

import log;
import algebra;
import image;

namespace tire {

using namespace algebra;

export struct Billboard final {
    using value_type = float;
    using vector2_type = vector2<value_type>;
    using vector3_type = vector3<value_type>;
    using vector4_type = vector4<value_type>;

    auto setColor( const Colorf &value ) -> void {
        //
        color_ = value;
    }

    auto draw() -> void {
        const vector3_type topLeftVt = { -10.0f, -10.0f, 0.0f };
        const vector3_type topRightVt = { 10.0f, -10.0f, 0.0f };
        const vector3_type bottomRightVt = { 10.0f, 10.0f, 0.0f };
        const vector3_type bottomLeftVt = { -10.0f, 10.0f, 0.0f };

        quadVerticies_[0] = topLeftVt;
        quadVerticies_[1] = topRightVt;
        quadVerticies_[2] = bottomRightVt;
        quadVerticies_[3] = bottomRightVt;
        quadVerticies_[4] = bottomLeftVt;
        quadVerticies_[5] = topLeftVt;

        const vector2_type topLeftTc{ 0.0f, 0.0f };
        const vector2_type topRightTc{ 1.0f, 0.0f };
        const vector2_type bottomRightTc{ 1.0f, 1.0f };
        const vector2_type bottomLeftTc{ 0.0f, 1.0f };

        quadTexcrds_[0] = topLeftTc;
        quadTexcrds_[1] = topRightTc;
        quadTexcrds_[2] = bottomRightTc;
        quadTexcrds_[3] = bottomRightTc;
        quadTexcrds_[4] = bottomLeftTc;
        quadTexcrds_[5] = topLeftTc;

        const vector4_type color = color_.asVector4f();
        quadsColors_[0] = vector4f{ 1.0, 0.0, 0.0, 0.5 };  //color;
        quadsColors_[1] = vector4f{ 1.0, 0.0, 0.0, 0.5 };  //color;
        quadsColors_[2] = vector4f{ 1.0, 0.0, 0.0, 0.5 };  //color;
        quadsColors_[3] = vector4f{ 1.0, 0.0, 0.0, 0.5 };  //color;
        quadsColors_[4] = vector4f{ 1.0, 0.0, 0.0, 0.5 };  //color;
        quadsColors_[5] = vector4f{ 1.0, 0.0, 0.0, 0.5 };  //color;
    }

#define VERTICIES_PER_QUAD 6
#define JUST_SINGLE_QUAD 1

    [[nodiscard]]
    auto lettersCount() const -> size_t {
        //
        return JUST_SINGLE_QUAD;
    }

    [[nodiscard]] auto bufferVerticesSize() const -> size_t {
        //
        return JUST_SINGLE_QUAD * VERTICIES_PER_QUAD * sizeof( vector3_type );
    }

    [[nodiscard]] auto bufferTexcrdsSize() const -> size_t {
        //
        return JUST_SINGLE_QUAD * VERTICIES_PER_QUAD * sizeof( vector2_type );
    }

    [[nodiscard]] auto bufferVertclrsSize() const -> size_t {
        //
        return JUST_SINGLE_QUAD * VERTICIES_PER_QUAD * sizeof( vector4_type );
    }

    [[nodiscard]]
    auto verteciesData() const -> const vector3<value_type> * {
        return quadVerticies_.data();
    }

    [[nodiscard]]
    auto texcrdsData() const -> const vector2<value_type> * {
        return quadTexcrds_.data();
    }

    [[nodiscard]]
    auto clrsData() const -> const vector4<value_type> * {
        return quadsColors_.data();
    }

private:
    std::array<vector3_type, JUST_SINGLE_QUAD * VERTICIES_PER_QUAD> quadVerticies_{};
    std::array<vector2_type, JUST_SINGLE_QUAD * VERTICIES_PER_QUAD> quadTexcrds_{};
    std::array<vector4_type, JUST_SINGLE_QUAD * VERTICIES_PER_QUAD> quadsColors_{};

    Colorf color_{};
};

}  // namespace tire