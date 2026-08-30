module;

export module graph : switchgroup;

import : node;

namespace tire {

export struct Switch final : public Node {
    Switch( const Switch& other ) = delete;
    Switch( Switch&& other ) = delete;

    auto operator=( const Switch& other ) -> Switch& = delete;
    auto operator=( Switch&& other ) -> Switch& = delete;

private:
    ~Switch() = default;
};

}  // namespace tire
