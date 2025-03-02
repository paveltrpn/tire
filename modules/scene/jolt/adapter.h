
#pragma once

// Undef macros, defined in X.h (X11).
// Jolt have objects with similar names and this name
// expands as macros defined in X.h cause compile error.
#undef None
#undef Convex

#include "Jolt/Jolt.h"
#include <Jolt/Physics/PhysicsSystem.h>

// Define X11 macros again
#define None 0L
#define Convex 2

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
