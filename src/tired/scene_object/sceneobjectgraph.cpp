
#include "sceneobjectgraph.h"
#include "sceneobjectbase.h"

namespace tired {

SceneObjectGraph::SceneObjectGraph( SceneObjectBase* owner )
    : _owner{ owner } {
}

auto SceneObjectGraph::owner() const -> SceneObjectBase* {
    return _owner;
}

auto SceneObjectGraph::dmatrix() -> vsg::dmat4 {
    return this->matrix;
}

auto SceneObjectGraph::fmatrix() -> vsg::mat4 {
    return static_cast<vsg::mat4>( this->matrix );
}

}  // namespace tired
