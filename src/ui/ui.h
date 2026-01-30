
#pragma once

#include <memory>

#include <QQuickWidget>
#include <QMainWindow>

#include "appearance.h"
#include "tired/tired.h"

namespace tired {

struct TiredUi final : QMainWindow {
    Q_OBJECT
public:
    TiredUi( tired::Tired *tired, QObject *parent = nullptr );

private:
    tired::Tired *tired_{};

    QQmlEngine *engine_;
    QQmlContext *context_;

    QWidget *vsgWidget_{};
    QQuickWidget *topPanel_{};
    QQuickWidget *leftPanel_{};

    Appearance *theme_;
};

}  // namespace tired