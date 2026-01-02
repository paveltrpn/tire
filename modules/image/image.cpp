module;

#include <utility>
#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <sstream>

export module image:image;

import log;
import :color;

namespace tire {

export enum class IMAGE_DEPTH { RGB = 24, RGBA = 32 };

export struct Image {
    [[nodiscard]]
    auto bpp() const -> int {
        //
        return bpp_;
    };

    [[nodiscard]]
    auto components() const -> int {
        //
        return components_;
    };

    [[nodiscard]]
    auto width() const -> int {
        //
        return width_;
    };

    [[nodiscard]]
    auto height() const -> int {
        //
        return height_;
    };

    [[nodiscard]]
    auto data() const -> uint8_t * {
        //
        return data_;
    };

    [[nodiscard]]
    auto asPPM() const -> std::string {
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

    /**
     * @brief Base class exports as simple PPM format encoded
     * as base64 string.
     */
    [[nodiscard]]
    virtual auto asBase64() const -> std::string {
        //
        return {};
    }

    /**
     * @brief Base class exports as simple PPM format.
     */
    virtual auto writeToFile( const std::string &path ) const -> void {
        const auto filePath = std::filesystem::path{ path };

        if ( std::filesystem::exists( filePath ) ) {
            log::info( "trying to overwrite existing file \"{}\"", filePath.filename().string() );
        }

        std::ofstream file{ filePath.string(), std::ios::trunc };

        file << asPPM();

        file.close();
    }

    virtual ~Image() {
        //
        delete[] data_;
    };

protected:
    Image() = default;

    Image( int32_t width, int32_t height, const Colori &dc )
        : height_{ height }
        , width_{ width } {
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

    explicit Image( const Image &other )
        : bpp_{ other.bpp_ }
        , components_{ bpp_ / 8 }
        , width_{ other.width_ }
        , height_{ other.height_ }
        , data_{ new uint8_t[width_ * height_ * components_] } {
        //
        std::copy( other.data_, other.data_ + width_ * height_ * components_, data_ );
    }

    explicit Image( Image &&other ) noexcept {
        bpp_ = std::exchange( other.bpp_, 0 );
        components_ = std::exchange( other.components_, 0 );
        width_ = std::exchange( other.width_, 0 );
        height_ = std::exchange( other.height_, 0 );
        data_ = std::exchange( other.data_, nullptr );
    }

    auto operator=( const Image &other ) noexcept -> Image & {
        Image{ other }.swap( *this );
        return *this;
    }

    auto operator=( Image &&other ) noexcept -> Image & {
        Image{ std::move( other ) }.swap( *this );
        return *this;
    }

private:
    auto swap( Image &other ) noexcept -> void {
        using std::swap;
        swap( bpp_, other.bpp_ );
        swap( components_, other.components_ );
        swap( width_, other.width_ );
        swap( height_, other.height_ );
        swap( data_, other.data_ );
    }

protected:
    int bpp_{};
    int components_{};
    int width_{};
    int height_{};
    uint8_t *data_{};
};

}  // namespace tire
