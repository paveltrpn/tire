#include <vsg/all.h>

#ifdef vsgXchange_FOUND
#include <vsgXchange/all.h>
#endif

#include <iostream>
#include <print>

#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QVBoxLayout>
#include <QGraphicsOpacityEffect>
#include <QLabel>
#include <QPushButton>

#include <vsgQt/Window.h>

#include "ui/ui.h"
#include "tired/tired.h"

int main( int argc, char* argv[] ) {
    QApplication application( argc, argv );

    vsg::CommandLine arguments( &argc, argv );

    // Set up vsg::Options to pass in filepaths, ReaderWriters and other IO
    // related options to use when reading and writing files.
    auto options = vsg::Options::create();
    options->fileCache = vsg::getEnv( "VSG_FILE_CACHE" );
    options->paths = vsg::getEnvPaths( "VSG_FILE_PATH" );
#ifdef vsgXchange_FOUND
    options->add( vsgXchange::all::create() );
#endif

    options->readOptions( arguments );

    auto windowTraits = vsg::WindowTraits::create();
    windowTraits->windowTitle = "vsgQt viewer";
    windowTraits->debugLayer = arguments.read( { "--debug", "-d" } );
    windowTraits->apiDumpLayer = arguments.read( { "--api", "-a" } );

    // windowTraits->samples
    windowTraits->width = 1920;
    windowTraits->height = 1080;
    // windowTraits->fullscreen = true;

    bool continuousUpdate = !arguments.read( { "--event-driven", "--ed" } );
    auto interval = arguments.value<int>( 8, "--interval" );

    if ( arguments.errors() ) {
        return arguments.writeErrorMessages( std::cerr );
    }

    if ( argc <= 1 ) {
        std::println( "Please specify a 3d model or image file on the command line." );
        return 1;
    }

    vsg::Path filename = arguments[1];

    // Main objects initialization.
    auto tired = std::make_unique<tired::Tired>();
    auto tiredUi = std::make_unique<tired::TiredUi>( tired.get(), filename, options );
    // ============================

    tired->viewerCompile( interval, continuousUpdate );
    tiredUi->show();

    return application.exec();
}
