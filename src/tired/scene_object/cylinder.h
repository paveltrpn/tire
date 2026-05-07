
#pragma once

#include "sceneobjectbase.h"

namespace tired::object {

struct Cylinder final : public SceneObjectBase {
    Q_OBJECT

    Q_PROPERTY( tired::CylinderObjectData data READ getData NOTIFY dataChanged FINAL )

public:
    Cylinder( const CylinderObjectData& data );

    auto data() const -> CylinderObjectData const& override;
    auto data() -> CylinderObjectData& override;

    tired::CylinderObjectData getData();

private:
    CylinderObjectData _data{};
};

}  // namespace tired::object