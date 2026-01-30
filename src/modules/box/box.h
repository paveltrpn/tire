
#pragma once

#include <vsg/all.h>

namespace tired {

struct Box final : vsg::MatrixTransform {
    Box( vsg::dvec3 pos = vsg::dvec3{ 0.0, 0.0, 0.0 }, vsg::dvec3 scale = vsg::dvec3{ 1.0, 1.0, 1.0 } );

private:
};

}  // namespace tired
