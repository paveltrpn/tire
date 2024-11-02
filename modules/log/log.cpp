
#include "log.h"

namespace tire::log {

void print_source( const std::source_location location ) {
    constexpr char preamble[] = "\033[3;34m[source] \033[0m";
    std::cout << preamble << "file: " << location.file_name() << '('
              << location.line() << ':' << location.column() << ") `"
              << location.function_name() << "`" << '\n';
}

}  // namespace tire::log
