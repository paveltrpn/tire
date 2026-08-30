
#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>

#include "image.h"
#include "log/log.h"

namespace tire {

Image::Image( int32_t width, int32_t height, const Colori& dc )
    : _height{ height }
    , _width{ width } {
    _bpp = static_cast<decltype( _bpp )>( IMAGE_DEPTH::RGBA );

    const auto components = _bpp / 8;

    _data = new uint8_t[_width * _height * components];

    // Zeroed canvas color.
    // std::fill( data_, data_ + width_ * height_ * components, 0 );

    // Fill with provided default color.
    for ( int j = 0; j < _width * _height; j++ ) {
        size_t base = j * components;
        _data[base + 0] = 0;  //dc.r();
        _data[base + 1] = 0;  //dc.g();
        _data[base + 2] = 0;  //dc.b();
    }
};

Image::Image( const Image& other )
    : _bpp{ other._bpp }
    , _components{ _bpp / 8 }
    , _width{ other._width }
    , _height{ other._height }
    , _data{ new uint8_t[_width * _height * _components] } {
    //
    std::copy( other._data, other._data + _width * _height * _components, _data );
}

Image::Image( Image&& other ) noexcept {
    _bpp = std::exchange( other._bpp, 0 );
    _components = std::exchange( other._components, 0 );
    _width = std::exchange( other._width, 0 );
    _height = std::exchange( other._height, 0 );
    _data = std::exchange( other._data, nullptr );
}

Image::~Image() {
    //
    delete[] _data;
};

auto Image::bpp() const -> int {
    //
    return _bpp;
};

auto Image::components() const -> int {
    //
    return _components;
};

auto Image::width() const -> int {
    //
    return _width;
};

auto Image::height() const -> int {
    //
    return _height;
};

auto Image::data() const -> uint8_t* {
    //
    return _data;
};

auto Image::asPPM() const -> std::string {
    auto stream = std::stringstream{};
    const auto components = _bpp / 8;

    // Write header.
    stream << "P3\n" << _width << ' ' << _height << "\n255\n";

    // Write body.
    for ( int j = 0; j < _width * _height; j++ ) {
        size_t base = j * components;
        const auto ir = static_cast<int>( _data[base + 0] );
        const auto ig = static_cast<int>( _data[base + 1] );
        const auto ib = static_cast<int>( _data[base + 2] );

        stream << ir << ' ' << ig << ' ' << ib << '\n';
    }

    stream.flush();

    return stream.str();
}

auto Image::asBase64() const -> std::string {
    //
    return {};
}

auto Image::writeToFile( const std::string& path ) const -> void {
    const auto filePath = std::filesystem::path{ path };

    if ( std::filesystem::exists( filePath ) ) {
        log::info()( "trying to overwrite existing file \"{}\"", filePath.filename().string() );
    }

    std::ofstream file{ filePath.string(), std::ios::trunc };

    file << asPPM();

    file.close();
}

}  // namespace tire
