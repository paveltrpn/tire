#include <vsg/all.h>
#include <vsg/app/Viewer.h>

#include <QHBoxLayout>
#include <QSplitter>
#include <QWidget>

#ifdef vsgXchange_FOUND
#include <vsgXchange/all.h>
#endif

#include "ui.h"

namespace tired {

// ====================================================================
// ========== TiredUi =================================================
// ====================================================================

TiredUi::TiredUi( tired::Tired *tired, QObject *parent )
    : tired_{ tired }
    , engine_{ new QQmlEngine{ this } }
    , context_{ engine_->rootContext() }
    , topPanel_{ new QQuickWidget{ engine_, this } }
    , leftPanel_{ new QQuickWidget{ engine_, this } }
    , theme_{ new Appearance{ this } } {
    //

    setGeometry( 0, 0, 1920, 1080 );

    qmlRegisterSingletonInstance( "Tire", 1, 0, "Appearence", theme_ );
    qmlRegisterSingletonInstance( "Tire", 1, 0, "Tired", tired_ );

    auto windowTraits = vsg::WindowTraits::create();
    windowTraits->windowTitle = "tire editor";

    // windowTraits->samples
    windowTraits->width = 1920;
    windowTraits->height = 1080;

    topPanel_->setSource( QUrl::fromLocalFile( "../src/ui/qml/TopPanel.qml" ) );
    topPanel_->setResizeMode( QQuickWidget::SizeRootObjectToView );

    leftPanel_->setSource( QUrl::fromLocalFile( "../src/ui/qml/LeftPanel.qml" ) );
    leftPanel_->setResizeMode( QQuickWidget::SizeRootObjectToView );

    auto centralWidget = new QWidget{ this };
    setCentralWidget( centralWidget );

    auto *vLayout = new QHBoxLayout{};
    vLayout->setContentsMargins( 0, 0, 0, 0 );
    vLayout->setSpacing( 0 );

    auto *vSplitter = new QSplitter{ this };
    vSplitter->setOrientation( Qt::Horizontal );
    vSplitter->setStyleSheet( "QSplitter::handle { background-color:  #2a2b3b; }" );
    vSplitter->setHandleWidth( 6 );

    centralWidget->setLayout( vLayout );

    auto *hLayout = new QVBoxLayout{};
    hLayout->setContentsMargins( 0, 0, 0, 0 );
    hLayout->setSpacing( 6 );

    auto rightElementsWidget = new QWidget{ this };

    rightElementsWidget->setLayout( hLayout );
    vSplitter->addWidget( leftPanel_ );
    vSplitter->addWidget( rightElementsWidget );
    vSplitter->setSizes( { 384, 1920 - 384 } );

    vLayout->addWidget( vSplitter );

    vsgWindow_ = new vsgQt::Window( tired->viewer(), windowTraits );
    vsgWindow_->setTitle( "title" );
    vsgWindow_->initializeWindow();

    tired->initCamera( vsgWindow_, windowTraits->width, windowTraits->height );
    vsgWidget_ = QWidget::createWindowContainer( vsgWindow_, this );

    bool continuousUpdate = false;  // arguments.read( { "--event-driven", "--ed" } );
    auto interval = 8;              // arguments.value<int>( 8, "--interval" );

    auto *hSplitter = new QSplitter{ this };
    hSplitter->setOrientation( Qt::Vertical );
    hSplitter->setStyleSheet( "QSplitter::handle { background-color: #2a2b3b; }" );
    hSplitter->setHandleWidth( 6 );

    hSplitter->addWidget( topPanel_ );
    hSplitter->addWidget( vsgWidget_ );
    hSplitter->setSizes( { 56, 1080 - 56 } );

    hLayout->addWidget( hSplitter );
}

}  // namespace tired