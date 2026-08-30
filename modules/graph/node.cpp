module;

#include <memory>

export module graph : node;

import : visitor;

namespace tire {

// ============================================================================
// =================== Node ===================================================
// ============================================================================

// NOLINTNEXTLINE(cppcoreguidelines-virtual-class-destructor)
export struct Node : public std::enable_shared_from_this<Node> {
public:
    Node() = default;

    Node( const Node& other ) = delete;
    Node( Node&& other ) = delete;

    auto operator=( const Node& other ) -> Node& = delete;
    auto operator=( Node&& other ) -> Node& = delete;

    virtual auto accept( Visitor& visitor ) -> void = 0;

protected:
    // Protected virtual - intentional for heap-only.
    virtual ~Node() = default;
};

}  // namespace tire
