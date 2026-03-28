
#pragma once

#include <QObject>

namespace tired {

struct Scenegraph final : public QObject {
    Q_OBJECT

public:
    Scenegraph();

private:
};

}  // namespace tired