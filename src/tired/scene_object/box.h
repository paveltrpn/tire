
#pragma once

#include "sceneobjectbase.h"

namespace tired::object {

struct Box final : public SceneObjectBase {
    Q_OBJECT

    Q_PROPERTY( SceneObjectData data READ getData WRITE setData NOTIFY dataChanged FINAL )

public:
    auto data() const -> SceneObjectData const& override;
    auto data() -> SceneObjectData& override;

    SceneObjectData getData();
    void setData( const SceneObjectData& value );

private:
    SceneObjectData _data{};
};

}  // namespace tired::object