
#include <iostream>

#include <QApplication>
#include <QStyleHints>
#include <QVulkanInstance>

#include "window.h"
#include "log/log.h"

#include "config/config.h"

int main( int argc, char* argv[] ) {
    new tire::Config{ "assets/config.json" };

    const QGuiApplication app( argc, argv );

    tire::log::info( "Qt version: {}", qVersion() );

    // Force use vulkan as backend renderer API.
    QQuickWindow::setGraphicsApi( QSGRendererInterface::Vulkan );

    // Instance to config and pass to qt.
    QVulkanInstance inst;

    // NOTE: have no effect.
    inst.setApiVersion( { 1, 2, 0 } );

    // inst.setLayers( { "VK_LAYER_KHRONOS_validation" } );

    // inst.setExtensions({"VK_KHR_surface"});

    if ( !inst.create() ) {
        qDebug() << "bad vk instance";
        return 1;
    }

    tire::MainWindow w{};

    w.setVulkanInstance( &inst );
    w.show();

    // Main loop starts here.
    return app.exec();
}
