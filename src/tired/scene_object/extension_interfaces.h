
#pragma once

namespace tired {

struct IHasMaterial {
    virtual auto getMaterial() -> void = 0;
};

// ==========================================================================================================

struct IHasCollision {
    virtual auto getShape() -> void = 0;
};

}  // namespace tired