
#include <iostream>

#include <QFont>

#include "appearance.h"

namespace tired {

Appearance::Appearance( QObject *parent )
    : QObject{ parent } {
    auto wp = QDir{ QDir::currentPath() };
    wp.cdUp();
    // Load color scheme.
    QFile file( wp.path() + QDir::separator() + "src/ui/qml/colors/colors.json" );
    file.open( QIODevice::ReadOnly | QIODevice::Text );
    const QByteArray data = file.readAll();

    QJsonParseError parseError;
    const QJsonDocument doc = QJsonDocument::fromJson( data, &parseError );
    if ( parseError.error != QJsonParseError::NoError ) {
        std::cout << std::format( "color theme json parse failed with message: {}",
                                  parseError.errorString().toStdString() );
        return;
    }

    const auto object = doc.object();
    colors_ = object["colors"].toObject();
    gaps_ = object["gaps"].toObject();
    radius_ = object["radius"].toObject();

    const auto fontName = object["main_font"].toString();
    const auto baseFontSize = object["base_font_size"].toInt();

    // Build fonts map
    fonts_["title_big"] = QFont{ fontName, baseFontSize + 12, QFont::ExtraBold };
    fonts_["title_accent"] = QFont{ fontName, baseFontSize + 8, QFont::ExtraBold };
    fonts_["title"] = QFont{ fontName, baseFontSize + 8, QFont::Medium };
    fonts_["subtitle_accent"] = QFont{ fontName, baseFontSize + 8, QFont::ExtraBold };
    fonts_["subtitle"] = QFont{ fontName, baseFontSize + 8, QFont::Medium };
    fonts_["text_body_accent"] = QFont{ fontName, baseFontSize + 4, QFont::ExtraBold };
    fonts_["text_body"] = QFont{ fontName, baseFontSize + 4, QFont::Medium };
    fonts_["label_accent"] = QFont{ fontName, baseFontSize + 2, QFont::ExtraBold };
    fonts_["label"] = QFont{ fontName, baseFontSize + 2, QFont::Medium };
    fonts_["subtext_accent"] = QFont{ fontName, baseFontSize, QFont::ExtraBold };
    fonts_["subtext"] = QFont{ fontName, baseFontSize, QFont::Medium };
}

}  // namespace tired
