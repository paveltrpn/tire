
#include <format>

#include <QApplication>
#include <QObject>
#include <QQmlComponent>
#include <QQmlEngine>
#include <QQuickWindow>
#include <QStringLiteral>
#include <QtQml>

#include "config/config.h"
#include "log/log.h"
#include "window.h"

import render;

namespace tire {

MainWindow::MainWindow( QQuickView* parent )
    : QQuickView{ parent }
    , engine_{ engine() }
    , context_{ rootContext() }
    , settings_{ new QSettings(
          workPath().path() + QDir::separator() + "settings.ini",
          QSettings::NativeFormat, this ) }
    , theme_{ new Appearance{ workPath(), this } }

{
    QSGRendererInterface* rif = this->rendererInterface();

    // We are not prepared for anything other than running with the RHI and its Vulkan backend.
    if ( rif->graphicsApi() != QSGRendererInterface::Vulkan ) {
        log::fatal(
            "MainWindow === qt render backend based not on Vulkan api!" );
    } else {
        log::info( "MainWindow === qt render backend assert success" );
    }

    log::info( "MainWindow === start... work path: {}",
               workPath().path().toStdString() );

    // Configure main window.
    setResizeMode( QQuickView::SizeRootObjectToView );

    // Pass Appearence object pointer to qml.
    qmlRegisterSingletonInstance( "Tire", 1, 0, "Appearence", theme_ );

    // Register RenderItem qml type. We instatiate item of
    // of this type only once in main.qml
    qmlRegisterType<RenderItem>( "Tire", 1, 0, "Render" );

    // Pass pointer to this object (MainWindow) to qml.
    // Available as "mainWindowHandle.doSomeAction()"
    context_->setContextProperty( "mainWindowHandle", this );

    // Dummy context property registration. This action
    // supress qml warnings when access "renderItemHandle" members in qml.
    // Later "renderItemHandle" property will be assigned with real
    // RenderItem pointer.
    context_->setContextProperty( "renderItemHandle", nullptr );

    // Setup RenderItem update interval.
    update_.setInterval( 1 );

    // Schedule actions on main component loading.
    connect(
        this, &QQuickView::statusChanged, this,
        [this]( QQuickView::Status status ) -> void {
            switch ( status ) {
                case QQuickView::Error: {
                    log::fatal(
                        "MainWindow === main QML component "
                        "error!" );
                    break;
                }
                case QQuickView::Loading: {
                    log::info(
                        "MainWindow === main QML component loading... " );
                    break;
                }
                    // Actions when main.qml properly loaded.
                case QQuickView::Ready: {
                    log::info( "MainWindow === main QML component ready." );

                    // Get main renderer item handle from QML.
                    // It istatiates only once in main.qml.
                    //
                    // TODO: Spawn this in c++ code and attach to
                    // qml scene graph somehow to avoid explicit definition
                    // of this object in main.qml
                    renderItemHandle_ = rootObject()->findChild<RenderItem*>();
                    if ( !renderItemHandle_ ) {
                        log::fatal(
                            "MainWindow === can't acquire renderer "
                            "handle!" );
                    }

                    // Pass pointer to RenderItem object back to qml.
                    // We do it despite RenderItem already instatiated in
                    // main.qml as Render{} item and available by its id.
                    // Can be accesed as "renderItemHandle.doSomeAction()"
                    context_->setContextProperty( "renderItemHandle",
                                                  renderItemHandle_ );

                    // Start update timer.
                    update_.start();

                    // Call updateWindow to redraw qml item.
                    connect( &update_, &QTimer::timeout, renderItemHandle_,
                             &RenderItem::updateWindow );

                    // Do some actions when render object completed.
                    connect(
                        renderItemHandle_, &RenderItem::renderInitialized, this,
                        []() -> void {
                            //
                            log::info( "MainWindow === render initialized!" );
                        } );
                    break;
                }
                case QQuickView::Null: {
                    log::warning(
                        "MainWindow === main QML component is NULL." );
                    break;
                }
            }
        } );

    // Load main QML component.
    setSource( QUrl( workPath().path() + QDir::separator() +
                     "apps/qt/qml/main.qml" ) );

    const auto configHandle = tire::Config::instance();
    const auto width = configHandle->get<int>( "window_width" );
    const auto height = configHandle->get<int>( "window_height" );
    const auto posx = configHandle->get<int>( "window_pos_x" );
    const auto posy = configHandle->get<int>( "window_pos_y" );

    setGeometry( { posx, posy, width, height } );

    // Set deefault or restore main window geometry.
    // const auto restoredGeometry = settings_->value(
    // "main_window_geometry", QRect( 300, 300, 1024, 1024 ) );
    // setGeometry( restoredGeometry.toRect() );

    // Process quit actions.
    connect( engine_, &QQmlEngine::quit, this,
             [this]() -> void { QApplication::quit(); } );
}

void MainWindow::closeEvent( QCloseEvent* ev ) {
    log::info( "quit" );

    // Save window geometry at close.
    settings_->setValue( "main_window_geometry", geometry() );
}

void MainWindow::keyPressEvent( QKeyEvent* ev ) {
    switch ( ev->key() ) {
        case Qt::Key_Escape: {
            QApplication::quit();
            break;
        }
        default: {
            break;
        }
    }
}

void MainWindow::keyReleaseEvent( QKeyEvent* ev ) {
}

void MainWindow::mouseMoveEvent( QMouseEvent* ev ) {
}

void MainWindow::mousePressEvent( QMouseEvent* ev ) {
}

void MainWindow::mouseReleaseEvent( QMouseEvent* ev ) {
}

void MainWindow::resizeEvent( QResizeEvent* ev ) {
    // qDebug() << ev->size();

    if ( renderItemHandle_ ) renderItemHandle_->updateSurface();
}

}  // namespace tire
