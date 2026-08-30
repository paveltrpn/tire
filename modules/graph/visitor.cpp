module;

export module graph : visitor;

// import : node;
// import : group;
// import : transformgroup;
// import : stategroup;

namespace tire {

export struct Node;
export struct Group;
export struct StateGroup;
export struct TransformGroup;

// ============================================================================
// =================== Visitor ================================================
// ============================================================================

export struct Visitor {
public:
    Visitor() = default;

    Visitor( const Visitor& other ) = delete;
    Visitor( Visitor&& other ) = delete;

    auto operator=( const Visitor& other ) -> Visitor& = delete;
    auto operator=( Visitor&& other ) -> Visitor& = delete;

    virtual ~Visitor() = default;

    virtual auto apply( Node& node ) -> void {}
    virtual auto apply( Group& node ) -> void {}
    virtual auto apply( StateGroup& node ) -> void {}
    virtual auto apply( TransformGroup& node ) -> void {}
};

}  // namespace tire
