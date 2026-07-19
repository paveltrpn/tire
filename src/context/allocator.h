
#pragma once

#include <vulkan/vulkan.h>

namespace tire {

struct VMAllocator final {
    VMAllocator( const VMAllocator &other ) = delete;
    VMAllocator( VMAllocator &&other ) = delete;

    auto operator=( const VMAllocator &other ) -> VMAllocator & = delete;
    auto operator=( VMAllocator &&other ) -> VMAllocator & = delete;

    ~VMAllocator() = default;
};

}  // namespace tire