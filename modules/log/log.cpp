
module;

#include <iostream>
#include <format>
#include <source_location>
#include <exception>

export module log:log;

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

export {
    //

#define ENABLE_INFO_OUTPUT true
    template <bool enable = ENABLE_INFO_OUTPUT, typename... Ts>
    void info( std::format_string<Ts...> msg, Ts&&... args ) {
        if constexpr ( enable ) {
            constexpr char preamble[] = "\033[3;32m[info] \033[0m\t\t";
            std::cout << preamble
                      << std::vformat( msg.get(),
                                       std::make_format_args( args... ) )
                      << "\n";
        }
    }

#define ENABLE_NOTICE_OUTPUT true
    template <bool enable = ENABLE_NOTICE_OUTPUT, typename... Ts>
    void notice( std::format_string<Ts...> msg, Ts&&... args ) {
        if constexpr ( enable || ENABLE_NOTICE_OUTPUT ) {
            constexpr char preamble[] = "\033[3;36m[notice] \033[0m\t";
            std::cout << preamble
                      << std::vformat( msg.get(),
                                       std::make_format_args( args... ) )
                      << "\n";
        }
    }

#define ENABLE_DEBUG_OUTPUT true
    template <bool enable = ENABLE_DEBUG_OUTPUT, typename... Ts>
    void debug( std::format_string<Ts...> msg, Ts&&... args ) {
        if constexpr ( enable ) {
            constexpr char preamble[] = "\033[3;35m[debug] \033[0m\t";
            std::cout << preamble
                      << std::vformat( msg.get(),
                                       std::make_format_args( args... ) )
                      << "\n";
        }
    }

#define ENABLE_WARNING_OUTPUT true
    template <bool enable = ENABLE_WARNING_OUTPUT, typename... Ts>
    void warning( std::format_string<Ts...> msg, Ts&&... args ) {
        if constexpr ( enable ) {
            constexpr char preamble[] = "\033[3;33m[warning] \033[0m\t";
            std::cout << preamble
                      << std::vformat( msg.get(),
                                       std::make_format_args( args... ) )
                      << "\n";
        }
    }

#define ENABLE_ERROR_OUTPUT true
    template <bool enable = ENABLE_ERROR_OUTPUT, typename... Ts>
    void error( std::format_string<Ts...> msg, Ts&&... args ) {
        if constexpr ( enable ) {
            constexpr char preamble[] = "\033[3;31m[error] \033[0m\t";
            std::cout << preamble
                      << std::vformat( msg.get(),
                                       std::make_format_args( args... ) )
                      << "\n";
        }
    }

    template <typename... Ts>
    void fatal( std::format_string<Ts...> msg, Ts&&... args ) {
        constexpr char preamble[] = "\033[3;31m[fatal] \033[0m\t";
        std::cout << preamble
                  << std::vformat( msg.get(), std::make_format_args( args... ) )
                  << "\n";

        // Terminate
        std::terminate();
    }

    void print_source( const std::source_location location ) {
        constexpr char preamble[] = "\033[3;34m[source] \033[0m";
        std::cout << preamble << "file: " << location.file_name() << '('
                  << location.line() << ':' << location.column() << ") `"
                  << location.function_name() << "`" << '\n';
    }
}

}  // namespace tire::log
