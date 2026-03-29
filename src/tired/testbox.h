
#pragma once

#include <vsg/all.h>

namespace tired {

struct Testbox final : vsg::StateGroup {
    Testbox();

    auto initPipeline() -> void;
    auto initDrawCommand() -> void;

private:
    vsg::ref_ptr<vsg::Group> _root{};
};

}  // namespace tired
