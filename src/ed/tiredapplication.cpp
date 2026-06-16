
#include <QMouseEvent>

#include <vsg/all.h>

#include "tiredapplication.h"

TiredApplication::TiredApplication( int &argc, char **argv )
    : QApplication( argc, argv ) {
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
    windowTraits->vulkanVersion = VK_MAKE_API_VERSION( 0, 1, 4, 0 );
    windowTraits->width = 1920;
    windowTraits->height = 1080;
    windowTraits->x = 100;
    windowTraits->y = 100;
    // windowTraits->clearColor = vsg::vec4{ 55.0f / 255.0f, 55.0f / 255.0f, 55.0f / 255.0f, 1.0f };
    // windowTraits->fullscreen = true;

    _tiredUI = new tired::TiredUI{ windowTraits, this };

    connect( this, &TiredApplication::gobalMousePostionChanged, _tiredUI, &tired::TiredUI::onGlobalMouseMove );

    setAttribute( Qt::AA_UseStyleSheetPropagationInWidgetStyles );
}

bool TiredApplication::notify( QObject *receiver, QEvent *event ) {
    if ( event->type() == QEvent::MouseMove ) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>( event );
        gobalMousePostionChanged( mouseEvent->globalPosition() );
    }

    return QApplication::notify( receiver, event );
}
