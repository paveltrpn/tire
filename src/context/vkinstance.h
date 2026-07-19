
#pragma once

namespace tire {

struct VKInstance final {
    VKInstance( const VKInstance &other ) = delete;
    VKInstance( VKInstance &&other ) = delete;

    auto operator=( const VKInstance &other ) -> VKInstance & = delete;
    auto operator=( VKInstance &&other ) -> VKInstance & = delete;

    ~VKInstance() = default;
private:

};

}
