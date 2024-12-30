
#include "canvas.h"
namespace tire {

int32_t ipart( float a ) {
    float rt = 0;

    std::modf( a, &rt );

    return int32_t( rt );
}

float fpart( float a ) {
    float tmp = 0;
    return std::modf( a, &tmp );
}

Canvas::Canvas( int32_t width, int32_t height ) {
    cnvs_height = height;
    cnvs_width = width;

    data = new uint8_t[cnvs_width * cnvs_height * bpp];

    // цвет холста по умолчанию
    std::fill( data, data + cnvs_width * cnvs_height * bpp, 0 );
    // std::memset(data, 128, cnvs_height*cnvs_width*bpp);
}

Canvas::~Canvas() {
    delete[] data;
}

void Canvas::Canvas::set_pen_size( int32_t size ) {
    pen_size = size;
}

void Canvas::set_pen_color( uint8_t r, uint8_t g, uint8_t b ) {
    pen_color_r = r;
    pen_color_g = g;
    pen_color_b = b;
}

void Canvas::put_pixel( int32_t x, int32_t y ) {
    if ( ( x < 0 ) || ( y < 0 ) || ( x >= cnvs_width ) || ( y >= cnvs_width ) )
        return;

    data[( ( x * bpp ) * cnvs_height + y * bpp ) + 0] = pen_color_r;
    data[( ( x * bpp ) * cnvs_height + y * bpp ) + 1] = pen_color_g;
    data[( ( x * bpp ) * cnvs_height + y * bpp ) + 2] = pen_color_b;
}

void Canvas::put_pixel_br( int32_t x, int32_t y, float br ) {
    if ( ( x < 0 ) || ( y < 0 ) || ( x >= cnvs_width ) || ( y >= cnvs_width ) )
        return;

    data[( ( x * bpp ) * cnvs_height + y * bpp ) + 0] =
        uint8_t( std::ceil( pen_color_r * br ) );
    data[( ( x * bpp ) * cnvs_height + y * bpp ) + 1] =
        uint8_t( std::ceil( pen_color_g * br ) );
    data[( ( x * bpp ) * cnvs_height + y * bpp ) + 2] =
        uint8_t( std::ceil( pen_color_b * br ) );
}

void Canvas::brasenham_line( std::pair<int32_t, int32_t> start,
                             std::pair<int32_t, int32_t> end ) {
    int32_t dX = abs( end.first - start.first );
    int32_t dY = abs( end.second - start.second );
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

    put_pixel( end.first, end.second );

    while ( ( now_point.first != end.first ) ||
            ( now_point.second != end.second ) ) {
        put_pixel( now_point.first, now_point.second );

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

void Canvas::wu_line( std::pair<int32_t, int32_t> start,
                      std::pair<int32_t, int32_t> end ) {
    float dx = end.first - start.first;
    float dy = end.second - start.second;
    float gradient, xend, yend, gap, inter;
    int32_t xpxl1, ypxl1, xpxl2, ypxl2, i;

    if ( fabs( dx ) > fabs( dy ) ) {
        if ( end.first < start.first ) {
            std::swap( start, end );
        }

        gradient = dy / dx;
        xend = std::round( start.first );
        yend = start.second + gradient * ( xend - start.first );
        gap = 1 - fpart( start.first + 0.5f );
        xpxl1 = xend;
        ypxl1 = ipart( yend );
        put_pixel_br( xpxl1, ypxl1, ( 1.0f - fpart( yend ) ) * gap );
        put_pixel_br( xpxl1, ypxl1 + 1, fpart( yend ) * gap );
        inter = yend + gradient;

        xend = std::round( end.first );
        yend = end.second + gradient * ( xend - end.first );
        gap = fpart( end.first + 0.5f );
        xpxl2 = xend;
        ypxl2 = ipart( yend );
        put_pixel_br( xpxl2, ypxl2, ( 1.0f - fpart( yend ) ) * gap );
        put_pixel_br( xpxl2, ypxl2 + 1, fpart( yend ) * gap );

        for ( i = xpxl1 + 1; i < xpxl2 - 1; i++ ) {
            put_pixel_br( i, ipart( inter ), 1.0f - fpart( inter ) );
            put_pixel_br( i, ipart( inter ) + 1, fpart( inter ) );
            inter = inter + gradient;
        }
    } else {
        if ( end.second < start.second ) {
            std::swap( start, end );
        }

        gradient = dx / dy;
        yend = std::round( start.second );
        xend = start.first + gradient * ( yend - start.second );
        gap = fpart( start.second + 0.5f );
        ypxl1 = yend;
        xpxl1 = ipart( xend );
        put_pixel_br( xpxl1, ypxl1, 1.0f - fpart( xend ) * gap );
        put_pixel_br( xpxl1 + 1, ypxl1, fpart( xend ) * gap );
        inter = xend + gradient;

        yend = std::round( end.second );
        xend = end.first + gradient * ( yend - end.second );
        gap = fpart( end.second + 0.5 );
        ypxl2 = yend;
        xpxl2 = ipart( xend );
        put_pixel_br( xpxl2, ypxl2, 1.0f - fpart( xend ) * gap );
        put_pixel_br( xpxl2 + 1, ypxl2, fpart( xend ) * gap );

        for ( i = ypxl1 + 1; i < ypxl2; i++ ) {
            put_pixel_br( ipart( inter ), i, 1.0f - fpart( inter ) );
            put_pixel_br( ipart( inter ) + 1, i, fpart( inter ) );
            inter += gradient;
        }
    }
}

void Canvas::dda_line( std::pair<int32_t, int32_t> start,
                       std::pair<int32_t, int32_t> end ) {
    int32_t dx = end.first - start.first;
    int32_t dy = end.second - start.second;
    int32_t steps =
        std::abs( dx ) > std::abs( dy ) ? std::abs( dx ) : std::abs( dy );

    float Xinc = dx / (float)steps;
    float Yinc = dy / (float)steps;

    auto X = (float)start.first;
    auto Y = (float)start.second;

    for ( int i = 0; i <= steps; i++ ) {
        put_pixel( X, Y );
        X += Xinc;
        Y += Yinc;
    }
}

void Canvas::brasenham_circle( std::pair<int32_t, int32_t> center,
                               int32_t rd ) {
    int32_t x = 0;
    int32_t y = rd;
    int32_t delta = 1 - 2 * rd;
    int error = 0;

    while ( y >= 0 ) {
        put_pixel( center.first + x, center.second + y );
        put_pixel( center.first + x, center.second - y );
        put_pixel( center.first - x, center.second + y );
        put_pixel( center.first - x, center.second - y );

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

int32_t Canvas::get_width() {
    return cnvs_width;
}

int32_t Canvas::get_height() {
    return cnvs_height;
}

int32_t Canvas::get_bpp() {
    return bpp;
}

uint8_t *Canvas::get_data() {
    return data;
}

}  // namespace tire
