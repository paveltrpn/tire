
#include <vsg/all.h>

#include "sphere.h"

namespace tired::object {

Sphere::Sphere( const SphereObjectData& data )
    : _data{ data } {
}

auto Sphere::data() const -> tired::SphereObjectData const& {
    return _data;
}

auto Sphere::data() -> tired::SphereObjectData& {
    return _data;
}

tired::SphereObjectData Sphere::getData() {
    return _data;
}

}  // namespace tired::object
