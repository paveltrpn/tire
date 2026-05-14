
#pragma once

#include <vsg/all.h>

#include <QQuickWidget>
#include <QMainWindow>

#include "qt_window/window.h"

#include "appearance.h"
#include "tired/tired.h"

namespace tired {

// ====================================================================
// ========== TiredUi =================================================
// ====================================================================

struct TiredUi final : QMainWindow {
    Q_OBJECT
public:
    TiredUi( vsg::ref_ptr<vsg::WindowTraits> traits, QObject *parent = nullptr );

    void onGlobalMouseMove( const QPointF &pos );

private:
    auto reigsterTypes() -> void;

private:
    std::unique_ptr<tired::Tired> tired_{};

    QQmlEngine *engine_;
    QQmlContext *context_;

    Window *vsgWindow_{};
    QWidget *vsgWidget_{};
    QQuickWidget *topPanel_{};
    QQuickWidget *leftPanel_{};

    Appearance *theme_;
};

}  // namespace tired