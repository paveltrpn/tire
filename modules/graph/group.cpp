module;

#include <memory>
#include <vector>

export module graph : group;

import : node;

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
