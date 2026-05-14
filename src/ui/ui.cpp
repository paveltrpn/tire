#include <vsg/all.h>

#include <QHBoxLayout>
#include <QSplitter>
#include <QWidget>

#include "ui.h"

namespace tired {

// ====================================================================
// ========== TiredUi =================================================
// ====================================================================

TiredUI::TiredUI( vsg::ref_ptr<vsg::WindowTraits> traits, QObject *parent )
    : tired_{ std::make_unique<tired::Tired>() }
    , engine_{ new QQmlEngine{ this } }
    , context_{ engine_->rootContext() }
    , topPanel_{ new QQuickWidget{ engine_, this } }
    , leftPanel_{ new QQuickWidget{ engine_, this } }
    , theme_{ new Appearance{ this } } {
    //

    tired_->registerTypes();

    engine_->addImageProvider( "TiredImageProvider", new TiredImageProvider{} );

    const auto topPanelHeight = theme_->getGap( "top_panel_height" );
    const auto leftPanelWidth = theme_->getGap( "left_panel_width" );
    const auto splitterBorderColor = theme_->getColor( "splitter_border_color" );
    const auto splitterHandleWidth = theme_->getGap( "quarter" );
    const auto clearColor = theme_->getColor( "clear_color" );

    qmlRegisterSingletonInstance( "Tire", 1, 0, "Appearence", theme_ );
    qmlRegisterSingletonInstance( "Tire", 1, 0, "Tired", tired_.get() );

    // Use this object for main window position and size (in particular).
    qmlRegisterSingletonInstance( "Tire", 1, 0, "MainWindow", this );

    setGeometry( traits->x, traits->y, traits->width, traits->height );

    // Remove native decoration.
    setWindowFlags( Qt::FramelessWindowHint );

    // Set transparent main window to use qml defined application
    // specific decoration.
    setAttribute( Qt::WA_TranslucentBackground );

    // VSG initialization.
    vsgWindow_ = new Window( tired_->viewer(), traits );
    vsgWindow_->setTitle( "title" );
    vsgWindow_->initializeWindow();

    tired_->init( vsgWindow_, traits->width, traits->height );

    // Qt widgets initialization.
    vsgWidget_ = QWidget::createWindowContainer( vsgWindow_, this );
    topPanel_->setSource( QUrl::fromLocalFile( "../src/ui/qml/panels/TopPanel.qml" ) );
    topPanel_->setResizeMode( QQuickWidget::SizeRootObjectToView );

    leftPanel_->setSource( QUrl::fromLocalFile( "../src/ui/qml/panels/LeftPanel.qml" ) );
    leftPanel_->setResizeMode( QQuickWidget::SizeRootObjectToView );

    //
    topPanel_->setClearColor( Qt::transparent );
    leftPanel_->setClearColor( Qt::transparent );

    auto centralWidget = new QWidget{ this };
    setCentralWidget( centralWidget );

    auto *vLayout = new QVBoxLayout{};
    vLayout->setContentsMargins( 0, 0, 0, 0 );

    auto *vSplitter = new QSplitter{ this };
    vSplitter->setOrientation( Qt::Vertical );
    vSplitter->setStyleSheet( QString{ "QSplitter::handle { background-color:  %1; }" }.arg( splitterBorderColor ) );
    vSplitter->setHandleWidth( splitterHandleWidth );

    centralWidget->setLayout( vLayout );

    auto *hLayout = new QHBoxLayout{};
    hLayout->setContentsMargins( 0, 0, 0, 0 );

    auto rightElementsWidget = new QWidget{ this };

    rightElementsWidget->setLayout( hLayout );
    vSplitter->addWidget( topPanel_ );
    vSplitter->addWidget( rightElementsWidget );
    vSplitter->setSizes( { topPanelHeight, 1080 - topPanelHeight } );

    vLayout->addWidget( vSplitter );

    auto *hSplitter = new QSplitter{ this };
    hSplitter->setOrientation( Qt::Horizontal );
    hSplitter->setStyleSheet( QString{ "QSplitter::handle { background-color:  %1; }" }.arg( splitterBorderColor ) );
    hSplitter->setHandleWidth( splitterHandleWidth );

    hSplitter->addWidget( leftPanel_ );
    hSplitter->addWidget( vsgWidget_ );
    hSplitter->setSizes( { leftPanelWidth, 1920 - leftPanelWidth } );

    hLayout->addWidget( hSplitter );

    this->show();
}

void TiredUI::onGlobalMouseMove( const QPointF &pos ) {
    tired_->setGlobalMousePosX( pos.x() );
    tired_->setGlobalMousePosY( pos.y() );
}

void TiredUI::moveWindow() {
    this->windowHandle()->startSystemMove();
}

}  // namespace tired