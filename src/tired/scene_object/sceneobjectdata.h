
#pragma once

#include <QObject>
#include <QJsonObject>

#include <vsg/all.h>

namespace tired {

struct SceneObjectData final {
    Q_GADGET

public:
    vsg::dvec3 position() const {
        //
        return _position;
    }

    vsg::dvec3 orientation() const {
        //
        return _orientation;
    }

    vsg::dvec3 scale() const {
        //
        return _scale;
    }

    void setPosition( vsg::dvec3 value ) {
        //
        _position = value;
    }

    void setOrientation( vsg::dvec3 value ) {
        //
        _orientation = value;
    }

    void setScale( vsg::dvec3 value ) {
        //
        _scale = value;
    }

    QJsonObject toJson() const {
        //
        return {};
    };

    void fromJson( const QJsonObject &data ) {
        //
    }

private:
    vsg::dvec3 _position{ 0.0, 0.0, 0.0 };
    vsg::dvec3 _orientation{ 0.0, 0.0, 0.0 };
    vsg::dvec3 _scale{ 1.0, 1.0, 1.0 };
};

}  // namespace tired

Q_DECLARE_METATYPE( tired::SceneObjectData )