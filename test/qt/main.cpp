
#include <QGuiApplication>

#include "qt_render/render_window.h"

int main( int argc, char **argv ) {
    QGuiApplication app( argc, argv );

    // Q_INIT_RESOURCE( assets );

    auto *w = new tire::qt::RenderWindow{ "../test/qt/main.qml" };
    w->resize( 600, 600 );
    w->show();

    return app.exec();
}
