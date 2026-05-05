
#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>

#include "image.h"
#include "log/log.h"

namespace tired {

Image::Image( int32_t width, int32_t height /*, const Colori &dc */ )
    : height_{ height }
    , width_{ width } {
    // NOTE: RGB
    bpp_ = static_cast<decltype( bpp_ )>( IMAGE_DEPTH::RGBA );

    const auto components = bpp_ / 8;

    data_ = new uint8_t[width_ * height_ * components];

    // Zeroed canvas color.
    // std::fill( data_, data_ + width_ * height_ * components, 0 );

    // Fill with provided default color.
    for ( int j = 0; j < width_ * height_; j++ ) {
        size_t base = j * components;
        data_[base + 0] = 0;  //dc.r();
        data_[base + 1] = 0;  //dc.g();
        data_[base + 2] = 0;  //dc.b();
    }
};

Image::Image( const Image& other )
    : bpp_{ other.bpp_ }
    , components_{ bpp_ / 8 }
    , width_{ other.width_ }
    , height_{ other.height_ }
    , data_{ new uint8_t[width_ * height_ * components_] } {
    //
    std::copy( other.data_, other.data_ + width_ * height_ * components_, data_ );
}

Image::Image( Image&& other ) noexcept {
    bpp_ = std::exchange( other.bpp_, 0 );
    components_ = std::exchange( other.components_, 0 );
    width_ = std::exchange( other.width_, 0 );
    height_ = std::exchange( other.height_, 0 );
    data_ = std::exchange( other.data_, nullptr );
}

Image::~Image() {
    //
    delete[] data_;
};

auto Image::bpp() const -> int {
    //
    return bpp_;
};

auto Image::components() const -> int {
    //
    return components_;
};

auto Image::width() const -> int {
    //
    return width_;
};

auto Image::height() const -> int {
    //
    return height_;
};

auto Image::data() const -> uint8_t* {
    //
    return data_;
};

auto Image::asPPM() const -> std::string {
    auto stream = std::stringstream{};
    const auto components = bpp_ / 8;

    // Write header.
    stream << "P3\n" << width_ << ' ' << height_ << "\n255\n";

    // Write body.
    for ( int j = 0; j < width_ * height_; j++ ) {
        size_t base = j * components;
        const auto ir = static_cast<int>( data_[base + 0] );
        const auto ig = static_cast<int>( data_[base + 1] );
        const auto ib = static_cast<int>( data_[base + 2] );

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

}  // namespace tired
