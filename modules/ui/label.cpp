
module;

#include <array>
#include <memory>

export module ui:label;

import algebra;
import image;

namespace tire {

using namespace algebra;

export struct Label final {
    using value_type = float;

    auto set_glyph_width( float value ) -> void {
        //
    }

    auto set_glyph_height( float value ) -> void {
        //
    }

    auto setGlyphScale( float value ) -> void {
        //
    }

    auto set_glyph_gap( float value ) -> void {
        //
    }

    auto set_text_position( float x, float y ) -> void {
        //
    }

    auto setColor( const Colorf &value ) -> void {
        //
    }

    auto resetStringParapeters() -> void {
        //
    }

    auto setLabelPos( float x, float y ) -> void {
        text_pos_x = x;
        text_pos_y = y;
    }

#define VERTEX_PER_LETTER 6
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
            offset = ( glyph_quad_wdt + glyph_quad_gap ) * static_cast<float>( i );

            glyph_x = string[i] % fontColumnCount;
            glyph_y = ( string[i] / fontColumnCount ) - 1;

            // Build character quad vertecies data.
            const algebra::vector3f topLeftVt = { ( offset + 0.0f ) + text_pos_x, 0.0f + text_pos_y, 0.0f };
            const algebra::vector3f topRightVt = { ( offset + glyph_quad_wdt ) + text_pos_x, 0.0f + text_pos_y, 0.0f };
            const algebra::vector3f bottomRightVt = {
              ( offset + glyph_quad_wdt ) + text_pos_x, -glyph_quad_hgt + text_pos_y, 0.0f };
            const algebra::vector3f bottomLeftVt = {
              ( offset + 0.0f ) + text_pos_x, -glyph_quad_hgt + text_pos_y, 0.0f };

            // Build character quad texture coordinates data.
            letterQuadsVertecies_[( i * VERTEX_PER_LETTER ) + 0] = topLeftVt;
            letterQuadsVertecies_[( i * VERTEX_PER_LETTER ) + 1] = topRightVt;
            letterQuadsVertecies_[( i * VERTEX_PER_LETTER ) + 2] = bottomRightVt;
            letterQuadsVertecies_[( i * VERTEX_PER_LETTER ) + 3] = bottomRightVt;
            letterQuadsVertecies_[( i * VERTEX_PER_LETTER ) + 4] = bottomLeftVt;
            letterQuadsVertecies_[( i * VERTEX_PER_LETTER ) + 5] = topLeftVt;

            const algebra::vector2f topLeftTc{ ( tc_gap_x * glyph_x ) + 0.0f, ( tc_gap_y * glyph_y ) + 0.0f };

            const algebra::vector2f topRightTc{ ( tc_gap_x * glyph_x ) + tc_gap_x, ( tc_gap_y * glyph_y ) + 0.0f };
            const algebra::vector2f bottomRightTc{
              ( tc_gap_x * glyph_x ) + tc_gap_x, ( tc_gap_y * glyph_y ) + tc_gap_y };

            const algebra::vector2f bottomLeftTc{ ( tc_gap_x * glyph_x ) + 0.0f, ( tc_gap_y * glyph_y ) + tc_gap_y };

            letterQuadsTexcrds_[( i * VERTEX_PER_LETTER ) + 0] = topLeftTc;
            letterQuadsTexcrds_[( i * VERTEX_PER_LETTER ) + 1] = topRightTc;
            letterQuadsTexcrds_[( i * VERTEX_PER_LETTER ) + 2] = bottomRightTc;
            letterQuadsTexcrds_[( i * VERTEX_PER_LETTER ) + 3] = bottomRightTc;
            letterQuadsTexcrds_[( i * VERTEX_PER_LETTER ) + 4] = bottomLeftTc;
            letterQuadsTexcrds_[( i * VERTEX_PER_LETTER ) + 5] = topLeftTc;

            // Build character quad color data.
            const auto color = color_.asVector4f();
            letterQuadsColors_[( i * VERTEX_PER_LETTER ) + 0] = color;
            letterQuadsColors_[( i * VERTEX_PER_LETTER ) + 1] = color;
            letterQuadsColors_[( i * VERTEX_PER_LETTER ) + 2] = color;
            letterQuadsColors_[( i * VERTEX_PER_LETTER ) + 3] = color;
            letterQuadsColors_[( i * VERTEX_PER_LETTER ) + 4] = color;
            letterQuadsColors_[( i * VERTEX_PER_LETTER ) + 5] = color;
            lettersCount_ += i;
        }
        // Save count of symbols that allready have been ocupied space in buffer.
    }

    auto lettersCount() -> size_t {
        //
        return lettersCount_;
    }

    [[nodiscard]] auto bufferVerticesSize() const -> size_t {
        //
        return lettersCount_ * VERTEX_PER_LETTER * 3 * sizeof( value_type );
    }

    [[nodiscard]] auto bufferTexcrdsSize() const -> size_t {
        //
        return lettersCount_ * VERTEX_PER_LETTER * 2 * sizeof( value_type );
    }

    [[nodiscard]] auto bufferVertclrsSize() const -> size_t {
        //
        return lettersCount_ * VERTEX_PER_LETTER * 4 * sizeof( value_type );
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
    auto clrsData() const -> const algebra::vector4<value_type> * {
        return letterQuadsColors_.data();
    }

private:
// Size of "Letters buffer" - this is the number of all characters
// from all draw() calls that can one instance of this class operates
#define MAX_LETTERS_COUNT 64
    std::array<algebra::vector3<value_type>, MAX_LETTERS_COUNT * VERTEX_PER_LETTER> letterQuadsVertecies_{};
    std::array<algebra::vector2<value_type>, MAX_LETTERS_COUNT * VERTEX_PER_LETTER> letterQuadsTexcrds_{};
    std::array<algebra::vector4<value_type>, MAX_LETTERS_COUNT * VERTEX_PER_LETTER> letterQuadsColors_{};

    std::unique_ptr<Tga> fontImage_{};

    float glyphScale_{ 1.3f };
#define GLYPH_WIDTH 0.4f
#define GLYPH_HEIGHT 1.55f
    float glyph_quad_wdt{ GLYPH_WIDTH * glyphScale_ };
    float glyph_quad_hgt{ GLYPH_HEIGHT * glyphScale_ };
#define GLYPH_GAP 0.0f
    float glyph_quad_gap{ GLYPH_GAP };

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