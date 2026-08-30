module;

#include <memory>

export module graph : node;

namespace tire {

// ============================================================================
// =================== Node ===================================================
// ============================================================================

// NOLINTNEXTLINE(cppcoreguidelines-virtual-class-destructor)
export struct Node : public std::enable_shared_from_this<Node> {
    Node( const Node& other ) = delete;
    Node( Node&& other ) = delete;

    auto operator=( const Node& other ) -> Node& = delete;
    auto operator=( Node&& other ) -> Node& = delete;

protected:
    // Protected virtual - intentional for heap-only
    virtual ~Node() = default;
};

}  // namespace tire
