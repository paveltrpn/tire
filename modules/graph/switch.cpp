module;

#include <memory>
#include <vector>

export module graph : switchgroup;

import : node;
import : visitor;

namespace tire {

// ============================================================================
// =================== Switch =================================================
// ============================================================================

/**
 */
export struct Switch final : public Node {
    Switch( const Switch& other ) = delete;
    Switch( Switch&& other ) = delete;

    auto operator=( const Switch& other ) -> Switch& = delete;
    auto operator=( Switch&& other ) -> Switch& = delete;

    auto accept( Visitor& visitor ) -> void override {
        //
        visitor.apply( *this );
    }

    auto traverse( Visitor& visitor ) -> void {
        for ( auto&& child : _children ) {
            child->accept( visitor );
        }
    }

private:
    ~Switch() = default;

private:
    // TODO: add visibility flag.
    std::vector<std::shared_ptr<Node>> _children{};
};

}  // namespace tire
