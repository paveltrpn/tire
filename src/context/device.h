
#pragma once

namespace tire {

struct VKDvice final {
    VKDvice( const VKDvice &other ) = delete;
    VKDvice( VKDvice &&other ) = delete;

    auto operator=( const VKDvice &other ) -> VKDvice & = delete;
    auto operator=( VKDvice &&other ) -> VKDvice & = delete;

    ~VKDvice() = default;

private:

};

}