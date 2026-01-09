
module;

#include <array>
#include <memory>

export module ui:label;

import algebra;
import image;

namespace tire {

struct Label final {
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

    auto draw( const std::string &string ) -> void {
        // Смещение квада с i-ым символом, зависит от ширины квадов и зазора между ними
        float offset;
        // Столбец, в котором находится символ
        int32_t glyph_x;
        // Строка, в котором находится символ
        int32_t glyph_y;
        // Размер ячейки с символом в долях текстурных координат по горизонтали
        float tc_gap_x = 1.0f / static_cast<float>( fontColumnCount );
        // Размер ячейки с символом в долях текстурных координат по вертикали
        float tc_gap_y = 1.0f / static_cast<float>( fontRowCount );

        size_t i{ 0 };
        for ( ; i < string.length(); ++i ) {
            offset = ( glyph_quad_wdt + glyph_quad_gap ) * i;

            glyph_x = string[i] % fontColumnCount;
            glyph_y = ( string[i] / fontColumnCount ) - 1;

            // Build character quad vertecies data.
            const algebra::vector2f topLeftVt = { ( offset + 0.0f ) + text_pos_x, 0.0f + text_pos_y };
            const algebra::vector2f topRightVt = { ( offset + glyph_quad_wdt ) + text_pos_x, 0.0f + text_pos_y };
            const algebra::vector2f bottomRightVt = {
              ( offset + glyph_quad_wdt ) + text_pos_x, -glyph_quad_hgt + text_pos_y };
            const algebra::vector2f bottomLeftVt = { ( offset + 0.0f ) + text_pos_x, -glyph_quad_hgt + text_pos_y };

            // Build character quad texture coordinates data.
            letterQuadsVertecies_[( ( bufferPos_ + i ) * 4 ) + 0] = topLeftVt;
            letterQuadsVertecies_[( ( bufferPos_ + i ) * 4 ) + 1] = topRightVt;
            letterQuadsVertecies_[( ( bufferPos_ + i ) * 4 ) + 2] = bottomRightVt;
            letterQuadsVertecies_[( ( bufferPos_ + i ) * 4 ) + 3] = bottomLeftVt;

            const algebra::vector2f topLeftTc{ ( tc_gap_x * glyph_x ) + 0.0f, ( tc_gap_y * glyph_y ) + 0.0f };

            const algebra::vector2f topRightTc{ ( tc_gap_x * glyph_x ) + tc_gap_x, ( tc_gap_y * glyph_y ) + 0.0f };
            const algebra::vector2f bottomRightTc{
              ( tc_gap_x * glyph_x ) + tc_gap_x, ( tc_gap_y * glyph_y ) + tc_gap_y };

            const algebra::vector2f bottomLeftTc{ ( tc_gap_x * glyph_x ) + 0.0f, ( tc_gap_y * glyph_y ) + tc_gap_y };

            letterQuadsTexcrds_[( ( bufferPos_ + i ) * 4 ) + 0] = topLeftTc;
            letterQuadsTexcrds_[( ( bufferPos_ + i ) * 4 ) + 1] = topRightTc;
            letterQuadsTexcrds_[( ( bufferPos_ + i ) * 4 ) + 2] = bottomRightTc;
            letterQuadsTexcrds_[( ( bufferPos_ + i ) * 4 ) + 3] = bottomLeftTc;

            // Build character quad color data.
            const auto color = color_.asVector3f();
            letterQuadsColors_[( ( bufferPos_ + i ) * 4 ) + 0] = color;
            letterQuadsColors_[( ( bufferPos_ + i ) * 4 ) + 1] = color;
            letterQuadsColors_[( ( bufferPos_ + i ) * 4 ) + 2] = color;
            letterQuadsColors_[( ( bufferPos_ + i ) * 4 ) + 3] = color;
        }
        // Save count of symbols that allready have been ocupied space in buffer,
        // because there can be many of draw() calls before flush() that actualy
        // render text geometry and set all counter and buffers to zero
        bufferPos_ += i;
    }

private:
// Size of "Letters buffer" - this is the number of all characters
// from all draw() calls that can one instance of this class operates
#define LETTERS_COUNT 512
    std::array<algebra::vector2f, LETTERS_COUNT * 4> letterQuadsVertecies_{};
    std::array<algebra::vector3f, LETTERS_COUNT * 4> letterQuadsColors_{};
    std::array<algebra::vector2f, LETTERS_COUNT * 4> letterQuadsTexcrds_{};

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

    // Must be set to zero in flush()
    size_t bufferPos_{};

    Colorf color_{};
};

}  // namespace tire