module;

#include <memory>
#include <vector>

export module graph : scene;

import : locale;

namespace tire {

// ============================================================================
// =================== Scene ==================================================
// ============================================================================

export struct Scene final {
    Scene( const Scene& other ) = delete;
    Scene( Scene&& other ) = delete;

    auto operator=( const Scene& other ) -> Scene& = delete;
    auto operator=( Scene&& other ) -> Scene& = delete;

    ~Scene() = default;

    auto walk() -> void {
        //
    }

    auto attachLocale( std::shared_ptr<Locale> child ) {
        //
        _locales.push_back( std::move( child ) );
    }

private:
    std::vector<std::shared_ptr<Locale>> _locales{};
};

}  // namespace tire
