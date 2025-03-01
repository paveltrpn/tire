
#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <GL/gl.h>

#include "image/tga.h"
#include "image/color.h"
#include "algebra/vector2.h"
#include "algebra/matrix4.h"

namespace tire {

struct ScreenStringBase {
    ScreenStringBase( const std::string& name );

    ScreenStringBase( ScreenStringBase& other ) = delete;
    ScreenStringBase( ScreenStringBase&& other ) = delete;
    ScreenStringBase& operator=( ScreenStringBase& other ) = delete;
    ScreenStringBase& operator=( ScreenStringBase&& other ) = delete;

    virtual ~ScreenStringBase() = default;

    void set_glyph_width( float value );
    void set_glyph_height( float value );
    void setGlyphScale( float value );
    void set_glyph_gap( float value );
    void set_text_position( float x, float y );
    void setColor( const Colorf& value );
    void resetStringParapeters();

    void draw( const std::string& string );

    virtual void flush() = 0;

protected:
// Size of "Letters buffer" - this is the number of all characters
// from all draw() calls that can one instance of this class operates
#define LETTERS_COUNT 512
    std::array<algebra::vector2f, LETTERS_COUNT * 4> letterQuadsVertecies_{};
    std::array<algebra::vector3f, LETTERS_COUNT * 4> letterQuadsColors_{};
    std::array<algebra::vector2f, LETTERS_COUNT * 4> letterQuadsTexcrds_{};

    std::unique_ptr<Tga> fontImage_{};

    float glyphScale_{ 2.0f };
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
