
#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <utility>

#include "color.h"

namespace tire {

enum class IMAGE_DEPTH { RGB = 24, RGBA = 32 };

struct Image {
    [[nodiscard]]
    auto bpp() const -> int {
        //
        return bpp_;
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
    auto data() const -> uint8_t* {
        //
        return data_;
    };

    [[nodiscard]]
    auto asPPM() const -> std::string;

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
    virtual auto writeToFile( const std::string& path ) const -> void;

    virtual ~Image() {
        //
        delete[] data_;
    };

protected:
    Image() = default;

    Image( int32_t width, int32_t height, const Colori& dc );

    explicit Image( const Image& other ) {
        bpp_ = other.bpp_;
        width_ = other.width_;
        height_ = other.height_;

        const auto components = bpp_ / 8;

        data_ = new uint8_t[width_ * height_ * components];

        std::copy( other.data_, other.data_ + width_ * height_ * components,
                   data_ );
    }

    explicit Image( Image&& other ) noexcept {
        bpp_ = std::exchange( other.bpp_, 0 );
        width_ = std::exchange( other.width_, 0 );
        height_ = std::exchange( other.height_, 0 );
        data_ = std::exchange( other.data_, nullptr );
    }

    auto operator=( const Image& other ) noexcept -> Image& {
        Image{ other }.swap( *this );
        return *this;
    }

    auto operator=( Image&& other ) noexcept -> Image& {
        Image{ std::move( other ) }.swap( *this );
        return *this;
    }

private:
    auto swap( Image& other ) noexcept -> void {
        using std::swap;
        swap( bpp_, other.bpp_ );
        swap( width_, other.width_ );
        swap( height_, other.height_ );
        swap( data_, other.data_ );
    }

protected:
    int bpp_{};
    int width_{};
    int height_{};
    uint8_t* data_{};
};

}  // namespace tire