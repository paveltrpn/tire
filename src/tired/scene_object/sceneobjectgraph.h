
#pragma once

#include <vsg/all.h>

namespace tired {

struct SceneObjectBase;

struct SceneObjectGraph : public vsg::Group {
public:
    SceneObjectGraph( SceneObjectBase *owner );

    auto owner() const -> SceneObjectBase *;

private:
    SceneObjectBase *_owner{};
};

}  // namespace tired
