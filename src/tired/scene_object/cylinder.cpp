
#include <vsg/all.h>

#include "cylinder.h"

namespace tired::object {

Cylinder::Cylinder( const CylinderObjectData& data )
    : _data{ data } {
}

auto Cylinder::data() const -> tired::CylinderObjectData const& {
    return _data;
}

auto Cylinder::data() -> tired::CylinderObjectData& {
    return _data;
}

tired::CylinderObjectData Cylinder::getData() {
    return _data;
}

}  // namespace tired::object