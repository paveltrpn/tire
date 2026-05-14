
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

struct TiredUI final : QMainWindow {
    Q_OBJECT
public:
    TiredUI( vsg::ref_ptr<vsg::WindowTraits> traits, QObject *parent = nullptr );

    void onGlobalMouseMove( const QPointF &pos );

    Q_INVOKABLE void moveWindow();

private:
    auto reigsterTypes() -> void;

private:
    std::unique_ptr<tired::Tired> _tired{};

    QQmlEngine *_engine;
    QQmlContext *_context;

    Window *_vsgWindow{};
    QWidget *_vsgWidget{};
    QQuickWidget *_topPanel{};
    QQuickWidget *_leftPanel{};

    Appearance *_theme;
};

}  // namespace tired