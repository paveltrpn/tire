
#include "window.h"

namespace tired {

MainWindow::MainWindow( QWidget *parent )
    : QQuickView() {
    setSource( QUrl::fromLocalFile( "../src/qml/Main.qml" ) );
    setGeometry( QRect{ 0, 0, 512, 512 } );
}

}  // namespace tired