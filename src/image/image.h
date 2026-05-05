#include <utility>
#include <string>
#include <cstdint>

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

    Image( int32_t width, int32_t height /*, const Colori &dc */ );
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

}  // namespace tired
