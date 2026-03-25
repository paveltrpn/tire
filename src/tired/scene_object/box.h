
#pragma once

#include "sceneobjectbase.h"

namespace tired::object {

struct Box final : public SceneObjectBase {
    Q_OBJECT

    Q_PROPERTY( tired::BoxObjectData data READ getData NOTIFY dataChanged FINAL )

public:
    Box( const BoxObjectData& data );

    auto data() const -> BoxObjectData const& override;
    auto data() -> BoxObjectData& override;

    tired::BoxObjectData getData();

private:
    BoxObjectData _data{};
};

}  // namespace tired::object