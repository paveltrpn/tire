
#pragma once

#include <QObject>
#include <QJsonObject>

#include <vsg/all.h>

namespace tired {

enum class SceneObjectTypeEnum {
    //
    BASE = 0,
    BOX = 1
};

struct SceneObjectData {
    Q_GADGET

    Q_PROPERTY( SceneObjectTypeEnum type READ type WRITE setType FINAL )
    Q_PROPERTY( QString name READ name WRITE setName FINAL )
    Q_PROPERTY( QUuid uid READ uid WRITE setUid FINAL )
    Q_PROPERTY( vsg::dvec3 position READ position WRITE setPosition FINAL )
    Q_PROPERTY( vsg::dvec3 orientation READ orientation WRITE setOrientation FINAL )
    Q_PROPERTY( vsg::dvec3 scale READ scale WRITE setScale FINAL )

public:
    SceneObjectTypeEnum type() const {
        //
        return _type;
    }

    QString name() const {
        //
        return _name;
    }

    QUuid uid() const {
        //
        return _uid;
    }

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

    void setType( SceneObjectTypeEnum value ) {
        //
        _type = value;
    }

    void setName( const QString &value ) {
        //
        _name = value;
    }

    void setUid( const QUuid &value ) {
        //
        _uid = value;
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

    virtual QJsonObject toJson() const {
        //
        return {};
    };

    virtual void fromJson( const QJsonObject &data ) {
        //
    }

private:
    SceneObjectTypeEnum _type{ SceneObjectTypeEnum::BASE };

    QString _name{ "OBJECT" };
    QUuid _uid{};

    vsg::dvec3 _position{ 0.0, 0.0, 0.0 };
    vsg::dvec3 _orientation{ 0.0, 0.0, 0.0 };
    vsg::dvec3 _scale{ 1.0, 1.0, 1.0 };
};

// ==========================================================================================================

struct BoxObject final : public SceneObjectData {
    Q_GADGET

public:
    QJsonObject toJson() const override {
        //
        return {};
    };

    void fromJson( const QJsonObject &data ) override {
        //
    }

private:
};

}  // namespace tired

Q_DECLARE_METATYPE( tired::SceneObjectTypeEnum )

Q_DECLARE_METATYPE( tired::SceneObjectData )
Q_DECLARE_METATYPE( tired::BoxObject )
