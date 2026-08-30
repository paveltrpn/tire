module;

export module graph : leaf;

import : node;

namespace tire {

export struct Leaf : public Node {
    Leaf( const Leaf& other ) = delete;
    Leaf( Leaf&& other ) = delete;

    auto operator=( const Leaf& other ) -> Leaf& = delete;
    auto operator=( Leaf&& other ) -> Leaf& = delete;

private:
    ~Leaf() = default;
};

}  // namespace tire
