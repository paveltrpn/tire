
#include <QHBoxLayout>

#include "ui.h"

namespace tired {

TiredUi::TiredUi( tired::Tired *tired, QObject *parent )
    : tired_{ tired }
    , engine_{ new QQmlEngine{ this } }
    , context_{ engine_->rootContext() }
    , topPanel_{ new QQuickWidget{ engine_, this } }
    , leftPanel_{ new QQuickWidget{ engine_, this } }
    , theme_{ new Appearance{ this } } {
    //

    setGeometry( 0, 0, 1920, 1080 );

    // Pass Appearence object pointer to qml.
    qmlRegisterSingletonInstance( "Tire", 1, 0, "Appearence", theme_ );
    qmlRegisterSingletonInstance( "Tire", 1, 0, "Tired", tired_ );

    topPanel_->setSource( QUrl::fromLocalFile( "../src/ui/qml/TopPanel.qml" ) );
    topPanel_->setResizeMode( QQuickWidget::SizeRootObjectToView );

    leftPanel_->setSource( QUrl::fromLocalFile( "../src/ui/qml/LeftPanel.qml" ) );
    leftPanel_->setResizeMode( QQuickWidget::SizeRootObjectToView );

    centralWidget_ = new QWidget{ this };
    setCentralWidget( centralWidget_ );

    auto *vLayout = new QVBoxLayout( this );
    vLayout->setContentsMargins( 0, 0, 0, 0 );
    vLayout->setSpacing( 0 );
    vLayout->addWidget( topPanel_, 1 );

    auto *hLayout = new QHBoxLayout( this );
    centralWidget_->setLayout( vLayout );
    hLayout->setContentsMargins( 0, 0, 0, 0 );
    hLayout->setSpacing( 0 );

    vLayout->addLayout( hLayout, 15 );

    auto windowTraits = vsg::WindowTraits::create();
    windowTraits->windowTitle = "vsgQt viewer";

    // windowTraits->samples
    windowTraits->width = 1920;
    windowTraits->height = 1080;

    tired->loadTestScene();

    auto vsgWindow = tired->initWindow( windowTraits, nullptr );
    auto vsgWidget = QWidget::createWindowContainer( vsgWindow, this );

    bool continuousUpdate = false;  //!arguments.read( { "--event-driven", "--ed" } );
    auto interval = 8;              // arguments.value<int>( 8, "--interval" );

    hLayout->addWidget( leftPanel_, 3 );
    hLayout->addWidget( vsgWidget, 11 );
}

}  // namespace tired