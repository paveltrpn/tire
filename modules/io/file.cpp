
module;

#include "log/log.h"
static constexpr bool DEBUG_OUTPUT_IO_FILE_CPP{ true };

export module io:file;
import event;

namespace tire::io {

export event::FilesystemWatchAwaitable<event::Task<void>> watchForFile(
    const std::string &path ) {
    return { event::Context::loop(), path };
}

}  // namespace tire::io
