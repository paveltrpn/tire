 
#pragma once

#include <QObject>

namespace tire::qt {

struct Render final : QObject {
        Render(QObject *parent = nullptr);
};

}
