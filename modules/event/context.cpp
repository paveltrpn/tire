
#include "context.h"

#include "log/log.h"
static constexpr bool DEBUG_OUTPUT_EVENT_CONTEXT_CPP{ true };

namespace tire::event {

std::unique_ptr<Context> Context::instance_ = nullptr;

}  // namespace tire::event
