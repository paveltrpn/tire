
#include "sceneobjectbase.h"

namespace tired {

SceneObjectBase::SceneObjectBase( QObject* parent )
    : QObject{ parent }
    , _thisObjectRoot{ new SceneObjectGraph{ this } } {
}

void SceneObjectBase::setSelected( bool value ) {
    _selected = value;
    emit selectedChanged();
}

bool SceneObjectBase::selected() {
    return _selected;
}

auto SceneObjectBase::root() const -> vsg::ref_ptr<SceneObjectGraph> {
    return _thisObjectRoot;
}

}  // namespace tired
