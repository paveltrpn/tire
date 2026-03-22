
#include "box.h"

namespace tired::object {

auto Box::data() const -> tired::SceneObjectData const& {
    return _data;
}

auto Box::data() -> tired::SceneObjectData& {
    return _data;
}

tired::SceneObjectData Box::getData() {
    return _data;
}

void Box::setData( const tired::SceneObjectData& value ) {
    _data = value;
    emit dataChanged();
}

}  // namespace tired::object