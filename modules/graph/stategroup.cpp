module;

export module graph : stategroup;

import : group;

namespace tire {

export struct StateGroup final : public Group {
    StateGroup( const StateGroup& other ) = delete;
    StateGroup( StateGroup&& other ) = delete;

    auto operator=( const StateGroup& other ) -> StateGroup& = delete;
    auto operator=( StateGroup&& other ) -> StateGroup& = delete;

    auto accept( Visitor& visitor ) -> void override {
        //
        visitor.apply( *this );
    }

private:
    ~StateGroup() = default;
};

}  // namespace tire
