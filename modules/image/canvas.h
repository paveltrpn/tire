
#pragma once

#include <utility>
#include <cmath>

#include "image.h"
#include "color.h"

namespace tire {

struct Canvas final : Image {
    Canvas( int32_t width, int32_t height,
            const Colori& dc = Colori{ "skyblue" } )
        : Image{ width, height, dc } {}

    explicit Canvas( const Image& other )
        : Image{ other } {}

    explicit Canvas( Image&& other )
        : Image{ std::move( other ) } {}

    auto operator=( const Image& other ) noexcept -> Canvas& {
        Canvas{ other }.swap( *this );
        return *this;
    }

    auto operator=( Image&& other ) noexcept -> Canvas& {
        Canvas{ std::move( other ) }.swap( *this );
        return *this;
    }

    auto setPenSize( int32_t size ) -> void {
        //
        penSize_ = size;
    }

    auto setPenColor( uint8_t r, uint8_t g, uint8_t b, uint8_t a ) -> void {
        penColorR_ = r;
        penColorG_ = g;
        penColorB_ = b;
        penColorA_ = a;
    }

    auto setPenColor( const Colori& color ) -> void {
        penColorR_ = color.r();
        penColorG_ = color.g();
        penColorB_ = color.b();
        penColorA_ = color.a();
    }

    auto putPixel( int32_t x, int32_t y ) -> void;

    auto putPixelBr( int32_t x, int32_t y, float br ) -> void;

    auto lineBrasenham( std::pair<int32_t, int32_t> start,
                        std::pair<int32_t, int32_t> end ) -> void;

    auto lineWu( std::pair<int32_t, int32_t> start,
                 std::pair<int32_t, int32_t> end ) -> void;

    auto lineDDA( std::pair<int32_t, int32_t> start,
                  std::pair<int32_t, int32_t> end ) -> void;

    auto circleBrasenham( std::pair<int32_t, int32_t> center, int32_t rd )
        -> void;

private:
    auto swap( Canvas& other ) noexcept -> void {
        std::swap( bpp_, other.bpp_ );
        std::swap( width_, other.width_ );
        std::swap( height_, other.height_ );
        std::swap( data_, other.data_ );
    }

private:
    int32_t penSize_{ 1 };

    uint8_t penColorR_{ 255 };
    uint8_t penColorG_{ 255 };
    uint8_t penColorB_{ 255 };
    uint8_t penColorA_{ 255 };
};

}  // namespace tire