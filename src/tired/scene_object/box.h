
#pragma once

#include "sceneobjectbase.h"

namespace tired::object {

struct Box final : public SceneObjectBase {
    Q_OBJECT

    Q_PROPERTY( tired::SceneObjectData data READ getData WRITE setData NOTIFY dataChanged FINAL )

public:
    Box( const SceneObjectData& data );

    auto data() const -> SceneObjectData const& override;
    auto data() -> SceneObjectData& override;

    tired::SceneObjectData getData();
    void setData( const tired::SceneObjectData& value );

private:
    SceneObjectData _data{};
};

}  // namespace tired::object