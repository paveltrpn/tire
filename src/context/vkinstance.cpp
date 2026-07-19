
#include "vkinstance.h"

namespace tire {

VKInstance::VKInstance() {

}

[[nodiscard]] auto VKInstance::get() -> VkInstance {
    return vkInstance_;
}

}