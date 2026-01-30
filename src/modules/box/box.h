
#pragma once

#include <vsg/all.h>

namespace tired {

struct Box final : vsg::MatrixTransform {
    Box( vsg::dvec3 pos, vsg::dvec3 scale );

private:
};

}  // namespace tired
