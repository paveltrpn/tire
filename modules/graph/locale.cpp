module;

#include <memory>
#include <vector>

#include "algebra/vector3.h"

export module graph : locale;

import : node;
import : visitor;
import : stategroup;

namespace tire {

// ============================================================================
// =================== Locale =================================================
// ============================================================================

export struct Locale final : public std::enable_shared_from_this<Locale> {
public:
    Locale( const Locale& other ) = delete;
    Locale( Locale&& other ) = delete;

    auto operator=( const Locale& other ) -> Locale& = delete;
    auto operator=( Locale&& other ) -> Locale& = delete;

    auto accept( Visitor& visitor ) -> void {
        //
        visitor.apply( *( _root.get() ) );
    }

    auto origin() -> algebra::vector3f {
        //
        return _origin;
    }

private:
    ~Locale() = default;

private:
    algebra::vector3f _origin{};

    std::shared_ptr<Group> _root{};
};

}  // namespace tire
