module;

#include <memory>

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
    Locale() = default;

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

    auto setOrigin( algebra::vector3f value ) -> void {
        //
        _origin = value;
    }

    auto attachStateGroup( std::shared_ptr<StateGroup> sg ) -> void {
        //
        _root->attach( std::move( sg ) );
    }

    auto detachStateGroup( const StateGroup* sg ) -> void {
        // /
        _root->detach( sg );
    }

private:
    ~Locale() = default;

private:
    algebra::vector3f _origin{};
    std::shared_ptr<Group> _root{};
};

}  // namespace tire
