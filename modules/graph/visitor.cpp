module;

export module graph : visitor;

namespace tire {

export struct Node;
export struct Group;
export struct StateGroup;
export struct TransformGroup;
export struct Switch;

// ============================================================================
// =================== Visitor ================================================
// ============================================================================

/**
 */
export struct Visitor {
public:
    Visitor() = default;

    Visitor( const Visitor& other ) = default;
    Visitor( Visitor&& other ) = default;

    auto operator=( const Visitor& other ) -> Visitor& = default;
    auto operator=( Visitor&& other ) -> Visitor& = default;

    virtual ~Visitor() = default;

    virtual auto apply( Node& node ) -> void {
        //
    }

    virtual auto apply( Group& node ) -> void {
        //
    }

    virtual auto apply( StateGroup& node ) -> void {
        //
    }

    virtual auto apply( TransformGroup& node ) -> void {
        //
    }

    virtual auto apply( Switch& node ) -> void {
        //
    }
};

}  // namespace tire
