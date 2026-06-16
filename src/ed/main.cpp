

#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QVBoxLayout>
#include <QGraphicsOpacityEffect>
#include <QLabel>
#include <QPushButton>

#include <meta>

#include "tiredapplication.h"

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

    return application.exec();
}
