module;

#include <memory>
#include <vector>
#include <algorithm>

export module graph : scene;

import : locale;
import : visitor;

namespace tire {

// ============================================================================
// =================== DrawVisitor ============================================
// ============================================================================

struct DrawVisitor final : public Visitor {};

// ============================================================================
// =================== Scene ==================================================
// ============================================================================

export struct Scene final {
public:
    Scene() = default;

    Scene( const Scene& other ) = delete;
    Scene( Scene&& other ) = delete;

    auto operator=( const Scene& other ) -> Scene& = delete;
    auto operator=( Scene&& other ) -> Scene& = delete;

    ~Scene() = default;

    auto walk() -> void {
        auto dv = DrawVisitor{};

        for ( auto&& locale : _locales ) {
            locale->accept( dv );
        }
    }

    auto attachLocale( std::shared_ptr<Locale> child ) -> void {
        //
        _locales.push_back( std::move( child ) );
    }

    auto detachLocale( const Locale* child ) -> void {
        auto r = std::remove_if( _locales.begin(), _locales.end(), [child]( const auto& item ) -> bool {
            //
            return item.get() == child;
        } );

        _locales.erase( r, _locales.end() );
    }

private:
    std::vector<std::shared_ptr<Locale>> _locales{};
};

}  // namespace tire
