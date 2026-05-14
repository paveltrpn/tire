#include <vsg/all.h>

#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QVBoxLayout>
#include <QGraphicsOpacityEffect>
#include <QLabel>
#include <QPushButton>

#include <meta>

#include "tiredapplication.h"
#include "ui/ui.h"

// struct Reflectable {};
// auto foo() -> void {
//     constexpr auto bar = ^^Reflectable;
//     constexpr std::string_view name_identifier = std::meta::identifier_of( bar );
//     std::println( "{}", name_identifier );
// }

// template <std::meta::info... Ms>
// struct Outer {
//     struct Inner;
//     consteval {
//         define_aggregate( ^^Inner, {
//                                        Ms... } );
//     }
// };

// template <std::meta::info... Ms>
// using Cls = Outer<Ms...>::Inner;

// template <class T, auto... Vs>
// inline constexpr auto construct_from = T{ Vs... };

// consteval auto parse( std::string_view key, int value ) -> std::meta::info {
//     auto member = std::meta::reflect_constant( data_member_spec( ^^int, {
//                                                                             .name = key } ) );
//     auto init = std::meta::reflect_constant( value );

//     auto type = std::meta::substitute( ^^Cls, {
//                                                   member } );
//     return std::meta::substitute( ^^construct_from, {
//                                                         type, init } );
// }

auto main( int argc, char* argv[] ) -> int {
    // foo();
    // static_assert( [:parse( "x", 1 ):].x == 1 );
    // static_assert( [:parse( "y", 2 ):].y == 2 );

    TiredApplication application( argc, argv );

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

    auto tiredUi = std::make_unique<tired::TiredUi>( windowTraits );

    return application.exec();
}
