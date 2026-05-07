
#pragma once

namespace tired {

struct IHasCollision {
    virtual auto getShape() -> void = 0;
};

}  // namespace tired
