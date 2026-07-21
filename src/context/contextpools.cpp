
#include "contextpools.h"

#include "vkinstance.h"
#include "device.h"

namespace tire {

ContextPools::ContextPools( const VKInstance *instance, const VKDevice *device )
    : _instance{ instance }
    , _device{ device } {
}

ContextPools::~ContextPools() {
}

}  // namespace tire

