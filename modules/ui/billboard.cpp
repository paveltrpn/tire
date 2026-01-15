
module;

#include <array>

export module ui:billboard;

import log;
import algebra;
import image;

namespace tire {

using namespace algebra;

#define VERTICIES_PER_QUAD 6

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

        letterQuadsVertecies_[0] = topLeftVt;
        letterQuadsVertecies_[1] = topRightVt;
        letterQuadsVertecies_[2] = bottomRightVt;
        letterQuadsVertecies_[3] = bottomRightVt;
        letterQuadsVertecies_[4] = bottomLeftVt;
        letterQuadsVertecies_[5] = topLeftVt;

        const vector2_type topLeftTc{ 0.0f, 0.0f };
        const vector2_type topRightTc{ 1.0f, 0.0f };
        const vector2_type bottomRightTc{ 1.0f, 1.0f };
        const vector2_type bottomLeftTc{ 0.0f, 1.0f };

        letterQuadsTexcrds_[0] = topLeftTc;
        letterQuadsTexcrds_[1] = topRightTc;
        letterQuadsTexcrds_[2] = bottomRightTc;
        letterQuadsTexcrds_[3] = bottomRightTc;
        letterQuadsTexcrds_[4] = bottomLeftTc;
        letterQuadsTexcrds_[5] = topLeftTc;

        const vector4_type color = color_.asVector4f();
        letterQuadsColors_[0] = color;
        letterQuadsColors_[1] = color;
        letterQuadsColors_[2] = color;
        letterQuadsColors_[3] = color;
        letterQuadsColors_[4] = color;
        letterQuadsColors_[5] = color;
    }

    [[nodiscard]]
    auto lettersCount() const -> size_t {
        //
        return 1;
    }

    [[nodiscard]] auto bufferVerticesSize() const -> size_t {
        //
        return 1 * VERTICIES_PER_QUAD * sizeof( vector3_type );
    }

    [[nodiscard]] auto bufferTexcrdsSize() const -> size_t {
        //
        return 1 * VERTICIES_PER_QUAD * sizeof( vector2_type );
    }

    [[nodiscard]] auto bufferVertclrsSize() const -> size_t {
        //
        return 1 * VERTICIES_PER_QUAD * sizeof( vector4_type );
    }

    [[nodiscard]]
    auto verteciesData() const -> const vector3<value_type> * {
        return letterQuadsVertecies_.data();
    }

    [[nodiscard]]
    auto texcrdsData() const -> const vector2<value_type> * {
        return letterQuadsTexcrds_.data();
    }

    [[nodiscard]]
    auto clrsData() const -> const vector4<value_type> * {
        return letterQuadsColors_.data();
    }

private:
    std::array<vector3_type, 1 * VERTICIES_PER_QUAD> letterQuadsVertecies_{};
    std::array<vector2_type, 1 * VERTICIES_PER_QUAD> letterQuadsTexcrds_{};
    std::array<vector4_type, 1 * VERTICIES_PER_QUAD> letterQuadsColors_{};

    Colorf color_{};
};

}  // namespace tire