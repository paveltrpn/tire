
#include <QHBoxLayout>

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

    centralWidget_ = new QWidget{ this };
    setCentralWidget( centralWidget_ );

    auto *layout = new QHBoxLayout( this );
    centralWidget_->setLayout( layout );
    layout->setContentsMargins( 0, 0, 0, 0 );
    layout->setSpacing( 0 );

    setGeometry( 0, 0, 1920, 1080 );

    auto windowTraits = vsg::WindowTraits::create();
    windowTraits->windowTitle = "vsgQt viewer";
    // windowTraits->debugLayer = arguments.read( { "--debug", "-d" } );
    // windowTraits->apiDumpLayer = arguments.read( { "--api", "-a" } );

    // windowTraits->samples
    windowTraits->width = 1920;
    windowTraits->height = 1080;

    tired->loadTestScene();

    auto vsgWindow = tired->initWindow( windowTraits, nullptr );
    auto vsgWidget = QWidget::createWindowContainer( vsgWindow, this );

    bool continuousUpdate = false;  //!arguments.read( { "--event-driven", "--ed" } );
    auto interval = 8;              // arguments.value<int>( 8, "--interval" );

    layout->addWidget( leftPanelWidget(), 3 );
    layout->addWidget( vsgWidget, 11 );
    layout->addWidget( rightPanelWidget(), 1 );

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