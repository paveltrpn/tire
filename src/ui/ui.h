
#pragma once

#include <memory>

#include <QQuickView>
#include <QQuickWidget>

#include "appearance.h"
#include "tired/tired.h"

namespace tired {

struct TiredUi final : QObject {
    Q_OBJECT
public:
    TiredUi( tired::Tired *tired, QObject *parent = nullptr );

    auto leftPanelWidget() const -> QQuickWidget *;
    auto rightPanelWidget() const -> QQuickWidget *;

private:
    tired::Tired *tired_{};

    QQmlEngine *engine_;
    QQmlContext *context_;

    std::shared_ptr<QQuickWidget> leftPanel_{};
    std::shared_ptr<QQuickWidget> rightPanel_{};

    Appearance *theme_;
};

}  // namespace tired