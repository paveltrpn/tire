module;

export module graph : locale;

import : node;

namespace tire {

export struct Locale final {
    Locale( const Locale& other ) = delete;
    Locale( Locale&& other ) = delete;

    auto operator=( const Locale& other ) -> Locale& = delete;
    auto operator=( Locale&& other ) -> Locale& = delete;

private:
    ~Locale() = default;
};

}  // namespace tire
