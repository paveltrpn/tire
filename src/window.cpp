
#include "window.h"

namespace tired {

MainWindow::MainWindow( QWidget *parent )
    : QMainWindow( parent ) {
    // ... setup menus, toolbars, etc. ...

    QQuickWidget *quickWidget = new QQuickWidget( this );
    quickWidget->setSource( QUrl::fromLocalFile( "../src/qml/Main.qml" ) );
    setCentralWidget( quickWidget );

    quickWidget->setGeometry( QRect{ 0, 0, 512, 512 } );

    // ... further setup ...
}

}  // namespace tired