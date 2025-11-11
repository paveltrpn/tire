module;

#include <cstdint>
#include <utility>
#include <cmath>

export module image:canvas;

import :image;
import :color;

namespace tire {

export struct Canvas final : Image {
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

    auto putPixel( int32_t x, int32_t y ) -> void {
        if ( ( x < 0 ) || ( y < 0 ) || ( x >= width_ ) || ( y >= height_ ) ) {
            return;
        }

        const auto components = bpp_ / 8;

        data_[( ( x * components ) * height_ + y * components ) + 0] =
            penColorR_;
        data_[( ( x * components ) * height_ + y * components ) + 1] =
            penColorG_;
        data_[( ( x * components ) * height_ + y * components ) + 2] =
            penColorB_;
    }

    auto putPixelBr( int32_t x, int32_t y, float br ) -> void {
        if ( ( x < 0 ) || ( y < 0 ) || ( x >= width_ ) || ( y >= height_ ) ) {
            return;
        }

        const auto components = bpp_ / 8;

        data_[( ( x * components ) * height_ + y * components ) + 0] =
            uint8_t( std::ceil( penColorR_ * br ) );
        data_[( ( x * components ) * height_ + y * components ) + 1] =
            uint8_t( std::ceil( penColorG_ * br ) );
        data_[( ( x * components ) * height_ + y * components ) + 2] =
            uint8_t( std::ceil( penColorB_ * br ) );
    }

    auto lineBrasenham( std::pair<int32_t, int32_t> start,
                        std::pair<int32_t, int32_t> end ) -> void {
        const auto dX = abs( end.first - start.first );
        const auto dY = abs( end.second - start.second );
        int32_t signX, signY;
        int32_t err2, err = dX - dY;
        std::pair<int32_t, int32_t> now_point = start;

        if ( start.first < end.first ) {
            signX = 1;
        } else {
            signX = -1;
        }

        if ( start.second < end.second ) {
            signY = 1;
        } else {
            signY = -1;
        }

        putPixel( end.first, end.second );

        while ( ( now_point.first != end.first ) ||
                ( now_point.second != end.second ) ) {
            putPixel( now_point.first, now_point.second );

            err2 = err * 2;

            if ( err2 > -dY ) {
                err -= dY;
                now_point.first += signX;
            }

            if ( err2 < dX ) {
                err += dX;
                now_point.second += signY;
            }
        }
    }

    auto lineWu( std::pair<int32_t, int32_t> start,
                 std::pair<int32_t, int32_t> end ) -> void {
        const auto dx = end.first - start.first;
        const auto dy = end.second - start.second;
        float xend, yend, gap, inter;
        int32_t xpxl1, ypxl1, xpxl2, ypxl2, i;

        auto ipart = []( float a ) -> int32_t {
            float rt = 0;
            std::modf( a, &rt );
            return int32_t( rt );
        };

        auto fpart = []( float a ) -> int32_t {
            float tmp = 0;
            return std::modf( a, &tmp );
        };

        if ( fabs( dx ) > fabs( dy ) ) {
            if ( end.first < start.first ) {
                std::swap( start, end );
            }

            const auto gradient =
                static_cast<float>( dy ) / static_cast<float>( dx );
            xend = std::round( start.first );
            yend = start.second + gradient * ( xend - start.first );
            gap = 1 - fpart( start.first + 0.5f );
            xpxl1 = xend;
            ypxl1 = ipart( yend );
            putPixelBr( xpxl1, ypxl1, ( 1.0f - fpart( yend ) ) * gap );
            putPixelBr( xpxl1, ypxl1 + 1, fpart( yend ) * gap );
            inter = yend + gradient;

            xend = std::round( end.first );
            yend = end.second + gradient * ( xend - end.first );
            gap = fpart( end.first + 0.5f );
            xpxl2 = xend;
            ypxl2 = ipart( yend );
            putPixelBr( xpxl2, ypxl2, ( 1.0f - fpart( yend ) ) * gap );
            putPixelBr( xpxl2, ypxl2 + 1, fpart( yend ) * gap );

            for ( i = xpxl1 + 1; i < xpxl2 - 1; i++ ) {
                putPixelBr( i, ipart( inter ), 1.0f - fpart( inter ) );
                putPixelBr( i, ipart( inter ) + 1, fpart( inter ) );
                inter = inter + gradient;
            }
        } else {
            if ( end.second < start.second ) {
                std::swap( start, end );
            }

            const auto gradient =
                static_cast<float>( dx ) / static_cast<float>( dy );
            yend = std::round( start.second );
            xend = start.first + gradient * ( yend - start.second );
            gap = fpart( start.second + 0.5f );
            ypxl1 = yend;
            xpxl1 = ipart( xend );
            putPixelBr( xpxl1, ypxl1, 1.0f - fpart( xend ) * gap );
            putPixelBr( xpxl1 + 1, ypxl1, fpart( xend ) * gap );
            inter = xend + gradient;

            yend = std::round( end.second );
            xend = end.first + gradient * ( yend - end.second );
            gap = fpart( end.second + 0.5 );
            ypxl2 = yend;
            xpxl2 = ipart( xend );
            putPixelBr( xpxl2, ypxl2, 1.0f - fpart( xend ) * gap );
            putPixelBr( xpxl2 + 1, ypxl2, fpart( xend ) * gap );

            for ( i = ypxl1 + 1; i < ypxl2; i++ ) {
                putPixelBr( ipart( inter ), i, 1.0f - fpart( inter ) );
                putPixelBr( ipart( inter ) + 1, i, fpart( inter ) );
                inter += gradient;
            }
        }
    }

    auto lineDDA( std::pair<int32_t, int32_t> start,
                  std::pair<int32_t, int32_t> end ) -> void {
        auto dx = end.first - start.first;
        auto dy = end.second - start.second;
        auto steps =
            std::abs( dx ) > std::abs( dy ) ? std::abs( dx ) : std::abs( dy );

        const auto Xinc = dx / static_cast<float>( steps );
        const auto Yinc = dy / static_cast<float>( steps );

        auto X = static_cast<float>( start.first );
        auto Y = static_cast<float>( start.second );

        for ( size_t i{ 0 }; i <= steps; i++ ) {
            putPixel( X, Y );
            X += Xinc;
            Y += Yinc;
        }
    }

    auto circleBrasenham( std::pair<int32_t, int32_t> center, int32_t rd )
        -> void {
        auto x = 0;
        auto y = rd;
        auto delta = 1 - 2 * rd;
        auto error = 0;

        while ( y >= 0 ) {
            putPixel( center.first + x, center.second + y );
            putPixel( center.first + x, center.second - y );
            putPixel( center.first - x, center.second + y );
            putPixel( center.first - x, center.second - y );

            error = 2 * ( delta + y ) - 1;

            if ( ( delta < 0 ) && ( error <= 0 ) ) {
                delta += 2 * ++x + 1;
                continue;
            }

            if ( ( delta > 0 ) && ( error > 0 ) ) {
                delta -= 2 * --y + 1;
                continue;
            }

            delta += 2 * ( ++x - --y );
        }
    }

private:
    auto swap( Canvas& other ) noexcept -> void {
        using std::swap;
        swap( bpp_, other.bpp_ );
        swap( width_, other.width_ );
        swap( height_, other.height_ );
        swap( data_, other.data_ );
    }

private:
    int32_t penSize_{ 1 };

    uint8_t penColorR_{ 255 };
    uint8_t penColorG_{ 255 };
    uint8_t penColorB_{ 255 };
    uint8_t penColorA_{ 255 };
};

}  // namespace tire
