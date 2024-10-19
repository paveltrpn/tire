
#include <QOpenGLContext>
#include <QQuickWindow>
#include <QQuickRenderControl>
#include <QQuickItem>
#include <QOpenGLFunctions>

#include <QQmlEngine>
#include <QQmlComponent>
#include <QQmlContext>

#include <QTimer>

#include "render_window.h"

namespace tire::qt
{
RenderWindow::RenderWindow( const QString &mainpath, QWindow *parent )
    : QWindow{ parent } {
    // set the window up
    setSurfaceType( QSurface::OpenGLSurface );

    QSurfaceFormat format;
    format.setMajorVersion( 3 );
    format.setMinorVersion( 3 );
    format.setProfile( QSurfaceFormat::CoreProfile );
    format.setDepthBufferSize( 24 );
    format.setStencilBufferSize( 8 );
    format.setSamples( 4 );

    setFormat( format );
    create();

    // create the GL context

    context_ = new QOpenGLContext( this );
    context_->setFormat( format );
    if ( !context_->create() )
        qFatal( "Unable to create context" );

    context_->makeCurrent( this );

    renderControl_ = new QQuickRenderControl( this );
    quickWindow_ = new QQuickWindow( renderControl_ );
    // quickWindow_->setClearBeforeRendering( false );

    QTimer *sceneSyncTimer = new QTimer( this );
    sceneSyncTimer->setInterval( 5 );
    sceneSyncTimer->setSingleShot( true );
    connect( sceneSyncTimer, &QTimer::timeout, this, &RenderWindow::syncScene );

    connect( renderControl_, &QQuickRenderControl::sceneChanged, sceneSyncTimer,
             static_cast<void ( QTimer::* )()>( &QTimer::start ) );

    connect( renderControl_, &QQuickRenderControl::renderRequested, this, &RenderWindow::draw );

    // renderControl_->initialize( context_ );
    renderControl_->initialize();

    QQmlEngine *engine = new QQmlEngine( this );

    if ( !engine->incubationController() )
        engine->setIncubationController( quickWindow_->incubationController() );

    // engine->rootContext()->setContextProperty( "_camera", m_camera );
    qmlComponent_ = new QQmlComponent( engine, this );

    connect( qmlComponent_, &QQmlComponent::statusChanged, this, &RenderWindow::onQmlComponentLoadingComplete );

    qmlComponent_->loadUrl( QUrl( mainpath ) );

    QTimer *redrawTimer = new QTimer( this );
    connect( redrawTimer, &QTimer::timeout, this, &RenderWindow::draw );
    redrawTimer->start( 500 );
}

RenderWindow::~RenderWindow() {
    context_->makeCurrent( this );

    delete rootItem_;
    delete qmlComponent_;
    delete renderControl_;
    delete quickWindow_;

    context_->doneCurrent();
    delete context_;
}

void RenderWindow::resizeEvent( QResizeEvent *e ) {
    // Simulate the "resize root item to follow window"
    updateRootItemSize();
    QWindow::resizeEvent( e );
}

void RenderWindow::mousePressEvent( QMouseEvent *e ) {
    qApp->sendEvent( quickWindow_, e );
    if ( !e->isAccepted() )
        QWindow::mousePressEvent( e );
}

void RenderWindow::mouseMoveEvent( QMouseEvent *e ) {
    qApp->sendEvent( quickWindow_, e );
    if ( !e->isAccepted() )
        QWindow::mousePressEvent( e );
}

void RenderWindow::mouseReleaseEvent( QMouseEvent *e ) {
    qApp->sendEvent( quickWindow_, e );
    if ( !e->isAccepted() )
        QWindow::mousePressEvent( e );
}

void RenderWindow::syncScene() {
    renderControl_->polishItems();
    renderControl_->sync();
    draw();
}

void RenderWindow::draw() {
    if ( !isExposed() )
        return;
    context_->makeCurrent( this );
    context_->functions()->glViewport( 0, 0, width() * devicePixelRatio(), height() * devicePixelRatio() );

    // renderer->render();

    // quickWindow_->resetOpenGLState();

    renderControl_->render();

    context_->swapBuffers( this );
}

void RenderWindow::onQmlComponentLoadingComplete() {
    if ( qmlComponent_->isLoading() )
        return;
    if ( qmlComponent_->isError() ) {
        const QList<QQmlError> errorList = qmlComponent_->errors();
        foreach ( const QQmlError &error, errorList )
            qWarning() << error.url() << error.line() << error;

        qFatal( "Unable to load QML file" );
    }

    QObject *rootObject = qmlComponent_->create();
    rootItem_ = qobject_cast<QQuickItem *>( rootObject );
    if ( !rootItem_ )
        qFatal( "Did not load a Qt Quick scene" );

    rootItem_->setParentItem( quickWindow_->contentItem() );
}

void RenderWindow::updateRootItemSize() {
    if ( rootItem_ ) {
        rootItem_->setWidth( width() );
        rootItem_->setHeight( height() );
    }

    quickWindow_->setHeight( height() );
    quickWindow_->setWidth( width() );
}

} // namespace tire::qt
