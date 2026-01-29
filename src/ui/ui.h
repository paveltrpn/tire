
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
    TiredUi( tired::Tired *tired, vsg::Path filename, vsg::ref_ptr<vsg::Options> options, QObject *parent = nullptr );

    auto leftPanelWidget() const -> QQuickWidget *;
    auto rightPanelWidget() const -> QQuickWidget *;

private:
    tired::Tired *tired_{};

    QQmlEngine *engine_;
    QQmlContext *context_;

    QWidget *centralWidget_{};
    std::shared_ptr<QQuickWidget> leftPanel_{};
    std::shared_ptr<QQuickWidget> rightPanel_{};

    Appearance *theme_;
};

}  // namespace tired