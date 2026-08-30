module;

export module graph : switchgroup;

import : node;
import : visitor;

namespace tire {

export struct Switch final : public Node {
    Switch( const Switch& other ) = delete;
    Switch( Switch&& other ) = delete;

    auto operator=( const Switch& other ) -> Switch& = delete;
    auto operator=( Switch&& other ) -> Switch& = delete;

    auto accept( Visitor& visitor ) -> void override {
        //
    }

private:
    ~Switch() = default;
};

}  // namespace tire
