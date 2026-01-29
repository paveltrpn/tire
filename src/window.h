
#pragma once

#include <QtWidgets/QMainWindow>
#include <QQuickView>

namespace tired {

class MainWindow : public QQuickView {
    Q_OBJECT
public:
    MainWindow( QWidget *parent = nullptr );
};

}  // namespace tired