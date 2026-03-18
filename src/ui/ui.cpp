#include <vsg/all.h>
#include <vsg/app/Viewer.h>

#include <QHBoxLayout>
#include <QSplitter>
#include <QWidget>

#include "ui.h"

namespace tired {

// ====================================================================
// ========== TiredUi =================================================
// ====================================================================

TiredUi::TiredUi( vsg::ref_ptr<vsg::WindowTraits> traits, QObject *parent )
    : tired_{ std::make_unique<tired::Tired>() }
    , engine_{ new QQmlEngine{ this } }
    , context_{ engine_->rootContext() }
    , topPanel_{ new QQuickWidget{ engine_, this } }
    , leftPanel_{ new QQuickWidget{ engine_, this } }
    , theme_{ new Appearance{ this } } {
    //

    const auto topPanelHeight = theme_->getGap( "top_panel_height" );
    const auto leftPanelWidth = theme_->getGap( "left_panel_width" );
    const auto splitterBorderColor = theme_->getColor( "splitter_border_color" );
    const auto splitterHandleWidth = theme_->getGap( "quarter" );
    const auto clearColor = theme_->getColor( "clear_color" );

    qmlRegisterSingletonInstance( "Tire", 1, 0, "Appearence", theme_ );
    qmlRegisterSingletonInstance( "Tire", 1, 0, "Tired", tired_.get() );

    setGeometry( traits->x, traits->y, traits->width, traits->height );

    // VSG initialization.
    vsgWindow_ = new Window( tired_->viewer(), traits );
    vsgWindow_->setTitle( "title" );
    vsgWindow_->initializeWindow();

    tired_->init( vsgWindow_, traits->width, traits->height );

    qmlRegisterSingletonInstance( "Tire", 1, 0, "Manipulator", tired_->manipulator() );
    qmlRegisterSingletonInstance( "Tire", 1, 0, "BasemeshSubraph", tired_->basemeshSubgraph() );
    qmlRegisterSingletonInstance( "Tire", 1, 0, "ObstaclesSubgraph", tired_->obstaclesSubgraph() );
    qmlRegisterSingletonInstance( "Tire", 1, 0, "ServiceObjectsSubgraph", tired_->serviceObjectsSubgraph() );

    // Qt widgets initialization.
    vsgWidget_ = QWidget::createWindowContainer( vsgWindow_, this );
    topPanel_->setSource( QUrl::fromLocalFile( "../src/ui/qml/TopPanel.qml" ) );
    topPanel_->setResizeMode( QQuickWidget::SizeRootObjectToView );

    leftPanel_->setSource( QUrl::fromLocalFile( "../src/ui/qml/LeftPanel.qml" ) );
    leftPanel_->setResizeMode( QQuickWidget::SizeRootObjectToView );

    auto centralWidget = new QWidget{ this };
    setCentralWidget( centralWidget );

    auto *vLayout = new QHBoxLayout{};
    vLayout->setContentsMargins( 0, 0, 0, 0 );

    auto *vSplitter = new QSplitter{ this };
    vSplitter->setOrientation( Qt::Horizontal );
    vSplitter->setStyleSheet( QString{ "QSplitter::handle { background-color:  %1; }" }.arg( splitterBorderColor ) );
    vSplitter->setHandleWidth( splitterHandleWidth );

    centralWidget->setLayout( vLayout );

    auto *hLayout = new QVBoxLayout{};
    hLayout->setContentsMargins( 0, 0, 0, 0 );

    auto rightElementsWidget = new QWidget{ this };

    rightElementsWidget->setLayout( hLayout );
    vSplitter->addWidget( leftPanel_ );
    vSplitter->addWidget( rightElementsWidget );
    vSplitter->setSizes( { leftPanelWidth, 1920 - leftPanelWidth } );

    vLayout->addWidget( vSplitter );

    auto *hSplitter = new QSplitter{ this };
    hSplitter->setOrientation( Qt::Vertical );
    hSplitter->setStyleSheet( QString{ "QSplitter::handle { background-color:  %1; }" }.arg( splitterBorderColor ) );
    hSplitter->setHandleWidth( splitterHandleWidth );

    hSplitter->addWidget( topPanel_ );
    hSplitter->addWidget( vsgWidget_ );
    hSplitter->setSizes( { topPanelHeight, 1080 - topPanelHeight } );

    hLayout->addWidget( hSplitter );

    this->show();
}

}  // namespace tired