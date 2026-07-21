
#pragma once

namespace tire {

struct VKInstance;
struct VKDevice;

struct ContextPools final {
    ContextPools( const VKInstance *instance,const VKDevice *device );

    ContextPools( const ContextPools &other ) = delete;
    ContextPools( ContextPools &&other ) = delete;

    auto operator=( const ContextPools &other ) -> ContextPools & = delete;
    auto operator=( ContextPools &&other ) -> ContextPools & = delete;

    ~ContextPools();

private:
    const VKInstance *_instance{};
    const VKDevice *_device{};

};

}

