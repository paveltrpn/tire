
#pragma once

#include <QObject>
#include <QJsonObject>
#include <QDir>
#include <QQuickImageProvider>

namespace tire {

// Application-wide image provide.
struct ImageProvider : QQuickImageProvider {
private:
};

// Colors and other theme related provider. Instance of
// this object available in qml.
struct Appearance : QObject {
    Q_OBJECT
    Q_PROPERTY( QJsonObject colors READ colors WRITE setColors MEMBER colors_
                    NOTIFY colorsChanged )
    Q_PROPERTY(
        QVariantMap fonts READ fonts MEMBER fonts_ NOTIFY colorsChanged )
    Q_PROPERTY( QVariantMap gaps READ gaps MEMBER gaps_ NOTIFY gapsChanged )
    Q_PROPERTY(
        QVariantMap radius READ radius MEMBER radius_ NOTIFY radiusChanged )

public:
    Appearance( const QDir &path, QObject *parent = nullptr );

    Q_INVOKABLE QJsonObject colors() { return colors_; };
    Q_INVOKABLE void setColors( QJsonObject &value ) { colors_ = value; };

    Q_INVOKABLE QVariantMap fonts() { return fonts_; }

    Q_INVOKABLE QVariantMap gaps() { return gaps_; }

    Q_INVOKABLE QVariantMap radius() { return radius_; }

signals:
    void colorsChanged();
    void fontsChanged();
    void gapsChanged();
    void radiusChanged();

private:
    QJsonObject colors_{};
    QVariantMap fonts_{};
    QVariantMap gaps_{};
    QVariantMap radius_{};
};

}  // namespace tire
