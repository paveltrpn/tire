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
    windowTraits->windowTitle = "tire editor";
    windowTraits->debugLayer = arguments.read( { "--debug", "-d" } );
    windowTraits->apiDumpLayer = arguments.read( { "--api", "-a" } );

    // windowTraits->samples
    windowTraits->width = 1920;
    windowTraits->height = 1080;
    windowTraits->clearColor = vsg::vec4{ 155.0f / 255.0f, 158.0f / 255.0f, 191.0f / 255.0f, 1.0f };
    // windowTraits->fullscreen = true;

    // Main objects initialization.
    auto tired = std::make_unique<tired::Tired>();
    auto tiredUi = std::make_unique<tired::TiredUi>( windowTraits, tired.get() );
    // ============================
    tiredUi->show();

    return application.exec();
}
