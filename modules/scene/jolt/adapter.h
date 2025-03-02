
#pragma once

#include "Jolt/Jolt.h"

#include <Jolt/Physics/PhysicsSystem.h>

namespace tire::jolt {
struct JoltAdapter final {
    JoltAdapter();

    JoltAdapter( const JoltAdapter& other ) = delete;
    JoltAdapter( JoltAdapter&& other ) = delete;

    JoltAdapter& operator=( JoltAdapter&& other ) = delete;
    JoltAdapter& operator=( const JoltAdapter& other ) = delete;

    ~JoltAdapter();

private:
    JPH::PhysicsSystem system_{};
};
}  // namespace tire::jolt
