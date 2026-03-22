
#pragma once

#include <QObject>

#include <vsg/all.h>

#include "sceneobjectgraph.h"
#include "sceneobjectdata.h"

namespace tired {

struct SceneObjectBase : public QObject {
    Q_OBJECT

    Q_PROPERTY( bool selected READ selected WRITE setSelected NOTIFY selectedChanged FINAL )

public:
    SceneObjectBase( QObject* parent = nullptr );

    Q_INVOKABLE void setSelected( bool value );
    Q_INVOKABLE bool selected();

    virtual auto data() const -> SceneObjectData const& = 0;
    virtual auto data() -> SceneObjectData& = 0;

signals:
    void dataChanged();
    void selectedChanged();

private:
    bool _selected{ false };

    vsg::ref_ptr<SceneObjectGraph> _root{};
};

}  // namespace tired