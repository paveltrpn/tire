
#include <iostream>
#include <string>
#include <format>

#include "screen_string_base.h"

import config;

namespace tire {

ScreenStringBase::ScreenStringBase( const std::string& name ) {
    const auto basePath = Config::instance()->getBasePath();
    const auto path =
        std::format( "{}/assets/img_fonts/{}", basePath.string(), name );
    fontImage_ = std::make_unique<Tga>( path );

    color_ = { "white" };
}

void ScreenStringBase::set_glyph_width( float value ) {
    glyph_quad_wdt = value;
}

void ScreenStringBase::set_glyph_height( float value ) {
    glyph_quad_hgt = value;
}

void ScreenStringBase::setGlyphScale( float value ) {
    glyphScale_ = value;
    glyph_quad_wdt *= glyphScale_;
    glyph_quad_hgt *= glyphScale_;
}

void ScreenStringBase::set_glyph_gap( float value ) {
    glyph_quad_gap = value;
}

void ScreenStringBase::set_text_position( float x, float y ) {
    text_pos_x = x;
    text_pos_y = y;
}

void ScreenStringBase::setColor( const Colorf& value ) {
    color_ = value;
}

void ScreenStringBase::resetStringParapeters() {
    glyphScale_ = 1.0f;
    glyph_quad_wdt = GLYPH_WIDTH;
    glyph_quad_hgt = GLYPH_HEIGHT;
    glyph_quad_gap = 0.0f;
    text_pos_x = 0.0f;
    text_pos_y = 0.0f;
}

void ScreenStringBase::draw( const std::string& string ) {
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

    size_t i{};
    for ( ; i < string.length(); ++i ) {
        offset = ( glyph_quad_wdt + glyph_quad_gap ) * i;

        glyph_x = string[i] % fontColumnCount;
        glyph_y = ( string[i] / fontColumnCount ) - 1;

        // Build character quad vertecies data
        const algebra::vector2f topLeftVt = { ( offset + 0.0f ) + text_pos_x,
                                              0.0f + text_pos_y };
        const algebra::vector2f topRightVt = {
            ( offset + glyph_quad_wdt ) + text_pos_x, 0.0f + text_pos_y };
        const algebra::vector2f bottomRightVt = {
            ( offset + glyph_quad_wdt ) + text_pos_x,
            -glyph_quad_hgt + text_pos_y };
        const algebra::vector2f bottomLeftVt = { ( offset + 0.0f ) + text_pos_x,
                                                 -glyph_quad_hgt + text_pos_y };

        // Build character quad texture coordinates data
        letterQuadsVertecies_[( ( bufferPos_ + i ) * 4 ) + 0] = topLeftVt;
        letterQuadsVertecies_[( ( bufferPos_ + i ) * 4 ) + 1] = topRightVt;
        letterQuadsVertecies_[( ( bufferPos_ + i ) * 4 ) + 2] = bottomRightVt;
        letterQuadsVertecies_[( ( bufferPos_ + i ) * 4 ) + 3] = bottomLeftVt;

        const algebra::vector2f topLeftTc{ ( tc_gap_x * glyph_x ) + 0.0f,
                                           ( tc_gap_y * glyph_y ) + 0.0f };

        const algebra::vector2f topRightTc{ ( tc_gap_x * glyph_x ) + tc_gap_x,
                                            ( tc_gap_y * glyph_y ) + 0.0f };
        const algebra::vector2f bottomRightTc{
            ( tc_gap_x * glyph_x ) + tc_gap_x,
            ( tc_gap_y * glyph_y ) + tc_gap_y };

        const algebra::vector2f bottomLeftTc{
            ( tc_gap_x * glyph_x ) + 0.0f, ( tc_gap_y * glyph_y ) + tc_gap_y };

        letterQuadsTexcrds_[( ( bufferPos_ + i ) * 4 ) + 0] = topLeftTc;
        letterQuadsTexcrds_[( ( bufferPos_ + i ) * 4 ) + 1] = topRightTc;
        letterQuadsTexcrds_[( ( bufferPos_ + i ) * 4 ) + 2] = bottomRightTc;
        letterQuadsTexcrds_[( ( bufferPos_ + i ) * 4 ) + 3] = bottomLeftTc;

        // Build character quad color data
        const auto color = color_.asVector3f();
        letterQuadsColors_[( ( bufferPos_ + i ) * 4 ) + 0] = color;
        letterQuadsColors_[( ( bufferPos_ + i ) * 4 ) + 1] = color;
        letterQuadsColors_[( ( bufferPos_ + i ) * 4 ) + 2] = color;
        letterQuadsColors_[( ( bufferPos_ + i ) * 4 ) + 3] = color;
    }
    // Save count of symbols that allready have been ocupied space in buffer,
    // because there can be many of draw() calls before flush() that actualy
    // render text geometry and set all counter and buffers to zero
    bufferPos_ = i;
}

}  // namespace tire