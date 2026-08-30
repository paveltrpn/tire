module;

#include <memory>

#include "algebra/vector3.h"

export module graph : locale;

import : node;

namespace tire {

// ============================================================================
// =================== Locale =================================================
// ============================================================================

export struct Locale final : public std::enable_shared_from_this<Locale> {
    Locale( const Locale& other ) = delete;
    Locale( Locale&& other ) = delete;

    auto operator=( const Locale& other ) -> Locale& = delete;
    auto operator=( Locale&& other ) -> Locale& = delete;

    auto origin() -> algebra::vector3f {
        //
        return _origin;
    }

private:
    ~Locale() = default;

private:
    algebra::vector3f _origin{};
};

}  // namespace tire
