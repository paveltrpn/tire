
#include <iostream>

#include <QFont>
#include <QString>

#include "appearance.h"
#include "log/log.h"

namespace tire {

Appearance::Appearance( const QDir& path, QObject* parent )
    : QObject{ parent } {
    // Load color scheme.
    QFile file( path.path() + QDir::separator() +
                "apps/qt/qml/assets/colors.json" );

    if ( !file.open( QIODevice::ReadOnly | QIODevice::Text ) ) {
        log::fatal( "Appearence === faild to open colors file!" );
    }

    const QByteArray data = file.readAll();

    QJsonParseError parseError;
    const QJsonDocument doc = QJsonDocument::fromJson( data, &parseError );
    if ( parseError.error != QJsonParseError::NoError ) {
        std::cout << std::format(
            "color theme json parse failed with message: {}",
            parseError.errorString().toStdString() );
        return;
    } else {
        // std::cout << doc.toJson( QJsonDocument::Indented ).toStdString();
        colors_ = doc.object();
    }

#define FONT_NAME "Monospace"
#define BASE_FONT_SIZE 9

    // Build fonts map
    fonts_["title_big"] =
        QFont{ FONT_NAME, BASE_FONT_SIZE + 12, QFont::ExtraBold };
    fonts_["title_accent"] =
        QFont{ FONT_NAME, BASE_FONT_SIZE + 8, QFont::ExtraBold };
    fonts_["title"] = QFont{ FONT_NAME, BASE_FONT_SIZE + 8, QFont::Medium };
    fonts_["subtitle_accent"] =
        QFont{ FONT_NAME, BASE_FONT_SIZE + 8, QFont::ExtraBold };
    fonts_["subtitle"] = QFont{ FONT_NAME, BASE_FONT_SIZE + 8, QFont::Medium };
    fonts_["text_body_accent"] =
        QFont{ FONT_NAME, BASE_FONT_SIZE + 4, QFont::ExtraBold };
    fonts_["text_body"] = QFont{ FONT_NAME, BASE_FONT_SIZE + 4, QFont::Medium };
    fonts_["label_accent"] =
        QFont{ FONT_NAME, BASE_FONT_SIZE + 2, QFont::ExtraBold };
    fonts_["label"] = QFont{ FONT_NAME, BASE_FONT_SIZE + 2, QFont::Medium };
    fonts_["subtext_accent"] =
        QFont{ FONT_NAME, BASE_FONT_SIZE, QFont::ExtraBold };
    fonts_["subtext"] = QFont{ FONT_NAME, BASE_FONT_SIZE, QFont::Medium };

    // Build gaps map
    gaps_["quarter"] = 4;
    gaps_["half"] = 8;
    gaps_["full"] = 16;

    // Build radius map
    radius_["quarter"] = 4;
    radius_["half"] = 8;
    radius_["full"] = 16;
}

}  // namespace tire
