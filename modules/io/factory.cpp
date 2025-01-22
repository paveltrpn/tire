
#include "factory.h"

#include "log/log.h"
static constexpr bool DEBUG_OUTPUT_IO_FACTORY_CPP{ true };

namespace tire::io {

std::unique_ptr<Factory> Factory::instance_ = nullptr;

}  // namespace tire::io
