
#include "window.h"

namespace tired {

MainWindow::MainWindow( QWidget *parent )
    : QQuickView() {
    setSource( QUrl::fromLocalFile( "../src/qml/Main.qml" ) );
}

}  // namespace tired