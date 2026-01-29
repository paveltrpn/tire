
#pragma once

#include <QtWidgets/QMainWindow>
#include <QtQuickWidgets/QQuickWidget>

namespace tired {

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr) ;
};

}