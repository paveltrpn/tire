
#pragma once

#include <vsg/all.h>

#include <QQuickWidget>
#include <QMainWindow>

#ifdef vsgXchange_FOUND
#include <vsgXchange/all.h>
#endif

#include <vsgQt/Window.h>

#include "appearance.h"
#include "tired/tired.h"

namespace tired {

// ====================================================================
// ========== Window ==================================================
// ====================================================================

struct Window final : vsgQt::Window {
public:
    Window( vsg::ref_ptr<vsgQt::Viewer> in_viewer, vsg::ref_ptr<vsg::WindowTraits> in_traits )
        : vsgQt::Window{ in_viewer, in_traits } {}

    Q_INVOKABLE QPoint mousePos() const;

    void mouseMoveEvent( QMouseEvent *event ) override;

private:
    QPoint mousePos_{};
};

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