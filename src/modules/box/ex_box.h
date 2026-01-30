
#pragma once

#include <vsg/all.h>

namespace tired {

struct ExBox final : vsg::MatrixTransform {
    ExBox( vsg::dvec3 pos, vsg::dvec3 scale );

private:
};

}  // namespace tired

