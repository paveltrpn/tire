
#pragma once

#include <vsg/all.h>

namespace tired {

struct Grid final : vsg::StateGroup {
    Grid();

    auto initPipeline() -> void;

private:
};

}  // namespace tired
