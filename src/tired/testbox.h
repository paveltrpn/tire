
#pragma once

#include <vsg/all.h>

namespace tired {

struct Testbox final : vsg::StateGroup {
    Testbox();

    auto initPipeline() -> void;

private:
};

}  // namespace tired

