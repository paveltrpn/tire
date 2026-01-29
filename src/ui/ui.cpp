
#include "ui.h"

namespace tired {

TiredUi::TiredUi( tired::Tired *tired, QObject *parent )
    : tired_{ tired }
    , engine_{ new QQmlEngine{ this } }
    , context_{ engine_->rootContext() }
    , leftPanel_{ new QQuickWidget{ engine_, nullptr } }
    , rightPanel_{ new QQuickWidget{ engine_, nullptr } }
    , theme_{ new Appearance{ this } } {
    //

    // Pass Appearence object pointer to qml.
    qmlRegisterSingletonInstance( "Tire", 1, 0, "Appearence", theme_ );
    qmlRegisterSingletonInstance( "Tire", 1, 0, "Tired", tired_ );

    leftPanel_->setSource( QUrl::fromLocalFile( "../src/ui/qml/LeftPanel.qml" ) );
    leftPanel_->setResizeMode( QQuickWidget::SizeRootObjectToView );

    rightPanel_->setSource( QUrl::fromLocalFile( "../src/ui/qml/RightPanel.qml" ) );
    rightPanel_->setResizeMode( QQuickWidget::SizeRootObjectToView );

    rightPanel_->setStyleSheet( "border: none;" );

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