
#include <QMouseEvent>

#include "tiredapplication.h"

bool TiredApplication::notify( QObject *receiver, QEvent *event ) {
    if ( event->type() == QEvent::MouseMove ) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>( event );
        qDebug() << "Global Mouse Move:" << mouseEvent->globalPosition();
    }
    return QApplication::notify( receiver, event );
}
