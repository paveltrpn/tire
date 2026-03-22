
#include "sceneobjectbase.h"

namespace tired {

SceneObjectBase::SceneObjectBase( QObject *parent )
    : QObject{ parent } {
}

void SceneObjectBase::setSelected( bool value ) {
    _selected = value;
    emit selectedChanged();
}

bool SceneObjectBase::selected() {
    return _selected;
}

}  // namespace tired
