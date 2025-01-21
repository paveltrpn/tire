
#pragma once

#include <iostream>
#include <string>
#include <format>
#include <source_location>

namespace tire::log {

/*
 * Example - "\033[1;32m[info] \033[0m"
 * here, \033 is the ESC character, ASCII 27. It is followed by [, then zero or more numbers separated by ;, and
 * finally the letter m. The numbers describe the colour and format to switch to from that point onwards.
 *
 * The codes for foreground and background colours are:
 *
 *          foreground background
 * black        30         40
 * red          31         41
 * green        32         42
 * yellow       33         43
 * blue         34         44
 * magenta      35         45
 * cyan         36         46
 * white        37         47
 *
 * Additionally, you can use these:
 *
 * reset             0  (everything back to normal)
 * bold/bright       1  (often a brighter shade of the same colour)
 * italic            3
 * underline         4
 * blink             5
 * inverse           7  (swap foreground and background colours)
 * crossed           9
 * bold/bright off  21
 * underline off    24
 * inverse off      27
 **/

template <bool enable = false, typename... Ts>
void info( std::format_string<Ts...> msg, Ts &&...args ) {
    if constexpr ( enable || ENABLE_INFO_OUTPUT ) {
        constexpr char preamble[] = "\033[3;32m[info] \033[0m\t\t";
        std::cout << preamble
                  << std::vformat( msg.get(), std::make_format_args( args... ) )
                  << "\n";
    }
}

template <bool enable = false, typename... Ts>
void notice( std::format_string<Ts...> msg, Ts &&...args ) {
    if constexpr ( enable || ENABLE_NOTICE_OUTPUT ) {
        constexpr char preamble[] = "\033[3;36m[notice] \033[0m\t";
        std::cout << preamble
                  << std::vformat( msg.get(), std::make_format_args( args... ) )
                  << "\n";
    }
}

template <bool enable, typename... Ts>
void debug( std::format_string<Ts...> msg, Ts &&...args ) {
    if constexpr ( enable || ENABLE_DEBUG_OUTPUT ) {
        constexpr char preamble[] = "\033[3;35m[debug] \033[0m\t";
        std::cout << preamble
                  << std::vformat( msg.get(), std::make_format_args( args... ) )
                  << "\n";
    }
}

template <bool enable = false, typename... Ts>
void warning( std::format_string<Ts...> msg, Ts &&...args ) {
    if constexpr ( enable || ENABLE_WARNING_OUTPUT ) {
        constexpr char preamble[] = "\033[3;33m[warning] \033[0m\t";
        std::cout << preamble
                  << std::vformat( msg.get(), std::make_format_args( args... ) )
                  << "\n";
    }
}

template <bool enable = false, typename... Ts>
void error( std::format_string<Ts...> msg, Ts &&...args ) {
    if constexpr ( enable || ENABLE_ERROR_OUTPUT ) {
        constexpr char preamble[] = "\033[3;31m[error] \033[0m\t";
        std::cout << preamble
                  << std::vformat( msg.get(), std::make_format_args( args... ) )
                  << "\n";
    }
}

void print_source(
    const std::source_location location = std::source_location::current() );

}  // namespace tire::log
