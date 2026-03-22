
#pragma once

#include <QObject>

#include <vsg/all.h>

namespace tired {

struct SceneObjectData {
    Q_GADGET

public:
private:
    vsg::dvec3 _position{};
    vsg::dvec3 _orientation{};
    vsg::dvec3 _scale{};
};

}  // namespace tired

Q_DECLARE_METATYPE( tired::SceneObjectData )