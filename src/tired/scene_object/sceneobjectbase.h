
#pragma once

#include <QObject>

namespace tired {

struct SceneObjectBase : public QObject {
    Q_OBJECT

public:
    SceneObjectBase( QObject *parent = nullptr );

private:
};

}  // namespace tired