
#include <iostream>

#include <QApplication>
#include <QStyleHints>
#include <QVulkanInstance>

#include "window.h"
#include "log/log.h"

#include "config/config.h"

auto main( int argc, char* argv[] ) -> int {
    new tire::Config{ "apps/config.json" };
    const auto configHandle = tire::Config::instance();

    const QGuiApplication app( argc, argv );

    tire::log::info( "Qt version: {}", qVersion() );

    // Force use vulkan as backend renderer API.
    QQuickWindow::setGraphicsApi( QSGRendererInterface::Vulkan );

    // Instance to config and pass to qt.
    QVulkanInstance inst;

    // NOTE: have no effect.
    inst.setApiVersion( { 1, 2, 0 } );

    if ( configHandle->get<bool>( "enable_validation_layers" ) ) {
        inst.setLayers( { "VK_LAYER_KHRONOS_validation" } );
    }

    if ( !inst.create() ) {
        tire::log::fatal( "bad vk instance" );
    }

    tire::MainWindow w{};

    w.setVulkanInstance( &inst );
    w.show();

    // Main loop starts here.
    return app.exec();
}
