#include <utility>
#include <string>
#include <cstdint>

#include "color.h"

namespace tired {

enum class IMAGE_DEPTH {
    //
    RGB = 24,
    RGBA = 32,
};

struct Image {
    [[nodiscard]] auto bpp() const -> int;
    [[nodiscard]] auto components() const -> int;
    [[nodiscard]] auto width() const -> int;
    [[nodiscard]] auto height() const -> int;
    [[nodiscard]] auto data() const -> uint8_t *;
    [[nodiscard]] auto asPPM() const -> std::string;

    /**
     * @brief Base class exports as simple PPM format encoded
     * as base64 string.
     */
    [[nodiscard]] virtual auto asBase64() const -> std::string;

    /**
     * @brief Base class exports as simple PPM format.
     */
    virtual auto writeToFile( const std::string &path ) const -> void;

    virtual ~Image();

protected:
    Image() = default;

    Image( int32_t width, int32_t height, const Colori &dc );
    explicit Image( const Image &other );
    explicit Image( Image &&other ) noexcept;

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
        swap( _bpp, other._bpp );
        swap( _components, other._components );
        swap( _width, other._width );
        swap( _height, other._height );
        swap( _data, other._data );
    }

protected:
    int _bpp{};
    int _components{};
    int _width{};
    int _height{};
    uint8_t *_data{};
};

}  // namespace tired
