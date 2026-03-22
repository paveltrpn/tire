
#include "box.h"

namespace tired::object {

auto Box::data() const -> tired::SceneObjectData const& {
}

auto Box::data() -> tired::SceneObjectData& {
}

tired::SceneObjectData Box::getData() {
    return _data;
}

void Box::setData( const tired::SceneObjectData& value ) {
    _data = value;
    emit dataChanged();
}

}  // namespace tired::object