
#pragma once

#include <vsg/all.h>

namespace tired {

struct SceneObjectBase;

struct SceneObjectGraph : public vsg::MatrixTransform {
public:
    SceneObjectGraph( SceneObjectBase *owner );

    auto owner() const -> SceneObjectBase *;

    auto dmatrix() -> vsg::dmat4;
    auto fmatrix() -> vsg::mat4;

private:
    SceneObjectBase *_owner{};
};

}  // namespace tired
