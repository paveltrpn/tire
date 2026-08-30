module;

#include <memory>

export module graph : group;

import : node;

namespace tire {

export struct Group : public Node {
    Group( const Group& other ) = delete;
    Group( Group&& other ) = delete;

    auto operator=( const Group& other ) -> Group& = delete;
    auto operator=( Group&& other ) -> Group& = delete;

protected:
    ~Group() override = default;
};

}  // namespace tire
