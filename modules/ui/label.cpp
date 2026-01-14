
module;

#include <array>
#include <memory>

export module ui:label;

import log;
import algebra;
import image;

namespace tire {

using namespace algebra;

export struct Label final {
    using value_type = float;
    using vector2_type = vector2<value_type>;
    using vector3_type = vector3<value_type>;
    using vector4_type = vector4<value_type>;

    auto set_glyph_width( float value ) -> void {
        //
    }

    auto set_glyph_height( float value ) -> void {
        //
    }

    auto setGlyphScale( float value ) -> void {
        //
    }

    auto setGlyphGap( float value ) -> void {
        //
        glyphGap_ = value;
    }

    auto set_text_position( float x, float y ) -> void {
        //
    }

    auto setColor( const Colorf &value ) -> void {
        //
        color_ = value;
    }

    auto resetStringParapeters() -> void {
        //
    }

    auto setLabelPos( float x, float y ) -> void {
        text_pos_x = x;
        text_pos_y = y;
    }

#define VERTICIES_PER_QUAD 6
    auto draw( const std::string &string ) -> void {
        // Смещение квада с i-ым символом, зависит от ширины квадов и зазора между ними
        float offset{};
        // Столбец, в котором находится символ
        int32_t glyph_x{};
        // Строка, в котором находится символ
        int32_t glyph_y{};
        // Размер ячейки с символом в долях текстурных координат по горизонтали
        float tc_gap_x = 1.0f / static_cast<float>( fontColumnCount );
        // Размер ячейки с символом в долях текстурных координат по вертикали
        float tc_gap_y = 1.0f / static_cast<float>( fontRowCount );

        for ( size_t i{ 0 }; i < string.length(); ++i ) {
            offset = ( glyph_quad_wdt + glyphGap_ ) * static_cast<float>( i );

            glyph_x = string[i] % fontColumnCount;
            glyph_y = ( string[i] / fontColumnCount ) - 1;

            // Build character quad vertecies data.
            const vector3_type topLeftVt = { ( offset + 0.0f ) + text_pos_x, 0.0f + text_pos_y, 0.0f };
            const vector3_type topRightVt = { ( offset + glyph_quad_wdt ) + text_pos_x, 0.0f + text_pos_y, 0.0f };
            const vector3_type bottomRightVt = {
              ( offset + glyph_quad_wdt ) + text_pos_x, -glyph_quad_hgt + text_pos_y, 0.0f };
            const vector3_type bottomLeftVt = { ( offset + 0.0f ) + text_pos_x, -glyph_quad_hgt + text_pos_y, 0.0f };

            // Build character quad texture coordinates data.
            letterQuadsVertecies_[( i * VERTICIES_PER_QUAD ) + 0] = topLeftVt;
            letterQuadsVertecies_[( i * VERTICIES_PER_QUAD ) + 1] = topRightVt;
            letterQuadsVertecies_[( i * VERTICIES_PER_QUAD ) + 2] = bottomRightVt;
            letterQuadsVertecies_[( i * VERTICIES_PER_QUAD ) + 3] = bottomRightVt;
            letterQuadsVertecies_[( i * VERTICIES_PER_QUAD ) + 4] = bottomLeftVt;
            letterQuadsVertecies_[( i * VERTICIES_PER_QUAD ) + 5] = topLeftVt;

            const vector2_type topLeftTc{ ( tc_gap_x * glyph_x ) + 0.0f, ( tc_gap_y * glyph_y ) + 0.0f };
            const vector2_type topRightTc{ ( tc_gap_x * glyph_x ) + tc_gap_x, ( tc_gap_y * glyph_y ) + 0.0f };
            const vector2_type bottomRightTc{ ( tc_gap_x * glyph_x ) + tc_gap_x, ( tc_gap_y * glyph_y ) + tc_gap_y };
            const vector2_type bottomLeftTc{ ( tc_gap_x * glyph_x ) + 0.0f, ( tc_gap_y * glyph_y ) + tc_gap_y };

            letterQuadsTexcrds_[( i * VERTICIES_PER_QUAD ) + 0] = topLeftTc;
            letterQuadsTexcrds_[( i * VERTICIES_PER_QUAD ) + 1] = topRightTc;
            letterQuadsTexcrds_[( i * VERTICIES_PER_QUAD ) + 2] = bottomRightTc;
            letterQuadsTexcrds_[( i * VERTICIES_PER_QUAD ) + 3] = bottomRightTc;
            letterQuadsTexcrds_[( i * VERTICIES_PER_QUAD ) + 4] = bottomLeftTc;
            letterQuadsTexcrds_[( i * VERTICIES_PER_QUAD ) + 5] = topLeftTc;

            // Build character quad color data.
            const vector4_type color = color_.asVector4f();
            letterQuadsColors_[( i * VERTICIES_PER_QUAD ) + 0] = color;
            letterQuadsColors_[( i * VERTICIES_PER_QUAD ) + 1] = color;
            letterQuadsColors_[( i * VERTICIES_PER_QUAD ) + 2] = color;
            letterQuadsColors_[( i * VERTICIES_PER_QUAD ) + 3] = color;
            letterQuadsColors_[( i * VERTICIES_PER_QUAD ) + 4] = color;
            letterQuadsColors_[( i * VERTICIES_PER_QUAD ) + 5] = color;
            lettersCount_++;
        }
    }

    [[nodiscard]]
    auto lettersCount() const -> size_t {
        //
        return lettersCount_;
    }

    [[nodiscard]] auto bufferVerticesSize() const -> size_t {
        //
        return lettersCount_ * VERTICIES_PER_QUAD * sizeof( vector3_type );
    }

    [[nodiscard]] auto bufferTexcrdsSize() const -> size_t {
        //
        return lettersCount_ * VERTICIES_PER_QUAD * sizeof( vector2_type );
    }

    [[nodiscard]] auto bufferVertclrsSize() const -> size_t {
        //
        return lettersCount_ * VERTICIES_PER_QUAD * sizeof( vector4_type );
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
// Size of "Letters buffer" - this is the number of all characters
// from all draw() calls that can one instance of this class operates
#define MAX_LETTERS_COUNT 64
    std::array<vector3_type, MAX_LETTERS_COUNT * VERTICIES_PER_QUAD> letterQuadsVertecies_{};
    std::array<vector2_type, MAX_LETTERS_COUNT * VERTICIES_PER_QUAD> letterQuadsTexcrds_{};
    std::array<vector4_type, MAX_LETTERS_COUNT * VERTICIES_PER_QUAD> letterQuadsColors_{};

    std::unique_ptr<Tga> fontImage_{};

    float glyphScale_{ 1.3f };
#define GLYPH_WIDTH 0.4f
#define GLYPH_HEIGHT 1.55f
    float glyph_quad_wdt{ GLYPH_WIDTH * glyphScale_ };
    float glyph_quad_hgt{ GLYPH_HEIGHT * glyphScale_ };
#define GLYPH_GAP 0.0f
    float glyphGap_{ GLYPH_GAP };

    float text_pos_x = { 0.0f };
    float text_pos_y = { 0.0f };

    // Формат шрифта - изображение TGA с началом сверху слева,
    // 32 столбца на 8 строк символов, первый символ - 32 ("пробел").
    // Размер ячейки с символом получается делением горизонтального и вертикального
    // размера изображения на количество столбцов и строк соответственно.
    int32_t fontColumnCount{ 32 };  // Количество столбцов символов в шрифте
    int32_t fontRowCount{ 8 };      // Количество строк символов в шрифте

    size_t lettersCount_{};

    Colorf color_{};
};

}  // namespace tire