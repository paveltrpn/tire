module;

#include <memory>
#include <vector>

export module graph : group;

import : node;
import : visitor;

namespace tire {

// ============================================================================
// =================== Group ==================================================
// ============================================================================

export struct Group : public Node {
public:
    Group() = default;

    Group( const Group& other ) = delete;
    Group( Group&& other ) = delete;

    auto operator=( const Group& other ) -> Group& = delete;
    auto operator=( Group&& other ) -> Group& = delete;

    auto accept( Visitor& visitor ) -> void override {
        //
        visitor.apply( *this );
    }

    auto traverse( Visitor& visitor ) -> void {
        for ( auto&& child : _children ) {
            child->accept( visitor );
        }
    }

    auto attach( std::shared_ptr<Node> child ) -> void {
        //
        _children.push_back( std::move( child ) );
    }

protected:
    ~Group() override = default;

private:
    std::vector<std::shared_ptr<Node>> _children{};
};

}  // namespace tire
