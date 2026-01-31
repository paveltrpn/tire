#include <vsg/all.h>

#ifdef vsgXchange_FOUND
#include <vsgXchange/all.h>
#endif

#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QVBoxLayout>
#include <QGraphicsOpacityEffect>
#include <QLabel>
#include <QPushButton>

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

    // Main objects initialization.
    auto tired = std::make_unique<tired::Tired>();
    auto tiredUi = std::make_unique<tired::TiredUi>( tired.get() );
    // ============================

    tired->viewerCompile( interval, continuousUpdate );
    tiredUi->show();

    return application.exec();
}
