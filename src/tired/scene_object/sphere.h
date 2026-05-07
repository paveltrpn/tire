
#pragma once

#include "sceneobjectbase.h"

namespace tired::object {

struct Sphere final : public SceneObjectBase {
    Q_OBJECT

    Q_PROPERTY( tired::SphereObjectData data READ getData NOTIFY dataChanged FINAL )

public:
    Sphere( const SphereObjectData& data );

    auto data() const -> SphereObjectData const& override;
    auto data() -> SphereObjectData& override;

    tired::SphereObjectData getData();

private:
    SphereObjectData _data{};
};

}  // namespace tired::object