
#pragma once

#include <memory>

#include <QQuickView>
#include <QQuickWidget>

namespace tired {

struct TiredUi final : QObject {
    Q_OBJECT
public:
    TiredUi( QObject *parent = nullptr );

    auto leftPanelWidget() const -> QQuickWidget *;
    auto rightPanelWidget() const -> QQuickWidget *;

private:
    std::shared_ptr<QQuickWidget> leftPanel_{};
    std::shared_ptr<QQuickWidget> rightPanel_{};
};

}  // namespace tired