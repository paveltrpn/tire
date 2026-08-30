module;

export module graph : leaf;

import : node;
import : visitor;

namespace tire {

export struct Leaf : public Node {
    Leaf( const Leaf& other ) = delete;
    Leaf( Leaf&& other ) = delete;

    auto operator=( const Leaf& other ) -> Leaf& = delete;
    auto operator=( Leaf&& other ) -> Leaf& = delete;

protected:
    ~Leaf() override = default;
};

}  // namespace tire
