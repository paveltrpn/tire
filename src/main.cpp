#include <vsg/all.h>

#ifdef vsgXchange_FOUND
#include <vsgXchange/all.h>
#endif

#include <iostream>

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
        std::cout << "Please specify a 3d model or image file on the command line." << std::endl;
        return 1;
    }

    vsg::Path filename = arguments[1];

    // Main objects initialization.
    auto tired = std::make_unique<tired::Tired>();
    auto tiredUi = std::make_unique<tired::TiredUi>();

    tired->loadTestScene( filename, options );

    auto* mainWindow = new QMainWindow{};
    auto* centralWidget = new QWidget{ mainWindow };

    auto* layout = new QHBoxLayout( mainWindow );
    centralWidget->setLayout( layout );
    layout->setContentsMargins( 0, 0, 0, 0 );
    layout->setSpacing( 0 );

    mainWindow->setGeometry( windowTraits->x, windowTraits->y, windowTraits->width, windowTraits->height );

    auto vsgWindow = tired->initWindow( windowTraits, nullptr );
    auto vsgWidget = QWidget::createWindowContainer( vsgWindow, mainWindow );

    layout->addWidget( tiredUi->leftPanelWidget(), 3 );
    layout->addWidget( vsgWidget, 11 );
    layout->addWidget( tiredUi->rightPanelWidget(), 1 );

    mainWindow->setCentralWidget( centralWidget );

    mainWindow->show();

    tired->viewerCompile( interval, continuousUpdate );

    return application.exec();
}
