
#include "image.h"

namespace tire {

Image::Image( int32_t width, int32_t height, const Colori& dc ) {
    height_ = height;
    width_ = width;

    // NOTE: RGB
    bpp_ = static_cast<decltype( bpp_ )>( IMAGE_DEPTH::RGB );

    const auto components = bpp_ / 8;

    data_ = new uint8_t[width_ * height_ * components];

    // Zeroed canvas color.
    // std::fill( data_, data_ + width_ * height_ * components, 0 );

    // Fill with provided default color.
    for ( int j = 0; j < width_ * height_; j++ ) {
        size_t base = j * components;
        data_[base + 0] = dc.r();
        data_[base + 1] = dc.g();
        data_[base + 2] = dc.b();
    }
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

auto Image::writeToFile( const std::string& path ) const -> void {
    const auto filePath = std::filesystem::path{ path };

    if ( std::filesystem::exists( filePath ) ) {
        std::cout << std::format( "trying to overwrite existing file \"{}\"",
                                  filePath.filename().string() );
    }

    std::ofstream file{ filePath.string(), std::ios::trunc };

    file << asPPM();

    file.close();
}

}  // namespace tire