
#include "ui.h"

namespace tired {

TiredUi::TiredUi( QObject *parent )
    : leftPanel_{ new QQuickWidget{} }
    , rightPanel_{ new QQuickWidget{} } {
    leftPanel_->setSource( QUrl::fromLocalFile( "../src/ui/qml/LeftPanel.qml" ) );
    leftPanel_->setResizeMode( QQuickWidget::SizeRootObjectToView );

    rightPanel_->setSource( QUrl::fromLocalFile( "../src/ui/qml/RightPanel.qml" ) );
    rightPanel_->setResizeMode( QQuickWidget::SizeRootObjectToView );

    // setAttribute( Qt::WA_TranslucentBackground );
    // setClearColor( Qt::transparent );
    // setAttribute( Qt::WA_AlwaysStackOnTop );
}

auto TiredUi::leftPanelWidget() const -> QQuickWidget * {
    return leftPanel_.get();
}
auto TiredUi::rightPanelWidget() const -> QQuickWidget * {
    return rightPanel_.get();
}

}  // namespace tired