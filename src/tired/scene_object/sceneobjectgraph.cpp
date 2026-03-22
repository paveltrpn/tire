
#include "sceneobjectgraph.h"
#include "sceneobjectbase.h"

namespace tired {

SceneObjectGraph::SceneObjectGraph( SceneObjectBase* owner )
    : _owner{ owner } {
}

auto SceneObjectGraph::owner() const -> SceneObjectBase* {
    return _owner;
}

}  // namespace tired
