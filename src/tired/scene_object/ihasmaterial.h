
#pragma once

namespace tired {

struct IHasMaterial {
    virtual auto getMaterial() -> void = 0;
};

}  // namespace tired