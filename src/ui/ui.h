
#pragma once

#include <vsg/all.h>

#include <QQuickWidget>
#include <QMainWindow>

#ifdef vsgXchange_FOUND
#include <vsgXchange/all.h>
#endif

#include "qt_window/Window.h"

#include "appearance.h"
#include "tired/tired.h"

namespace tired {

// ====================================================================
// ========== TiredUi =================================================
// ====================================================================

struct TiredUi final : QMainWindow {
    Q_OBJECT
public:
    TiredUi( tired::Tired *tired, QObject *parent = nullptr );

private:
    tired::Tired *tired_{};

    QQmlEngine *engine_;
    QQmlContext *context_;

    Window *vsgWindow_{};
    QWidget *vsgWidget_{};
    QQuickWidget *topPanel_{};
    QQuickWidget *leftPanel_{};

    Appearance *theme_;
};

}  // namespace tired