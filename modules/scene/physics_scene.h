
#pragma once

#include <filesystem>
#include <memory>

#include "nlohmann/json.hpp"
/*
// Undef macros, defined in X.h (X11).
// Jolt have objects with similar names and this name
// expands as macros defined in X.h cause compile error.
#undef None
#undef Convex

#include "Jolt/Jolt.h"
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/PhysicsSystem.h>

#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>
#include <Jolt/Physics/PhysicsSystem.h>

// Define X11 macros again
#define None 0L
#define Convex 2
*/
#include "body.h"
#include "camera.h"

namespace tire {

struct PhysicsScene {
    PhysicsScene( const std::filesystem::path &fname );

    PhysicsScene( const PhysicsScene &other ) = delete;
    PhysicsScene( PhysicsScene &&other ) = delete;
    PhysicsScene &operator=( const PhysicsScene &other ) = delete;
    PhysicsScene &operator=( PhysicsScene &&other ) = delete;

    virtual ~PhysicsScene() = default;

    void setActiveCamera( size_t id ) { activeCamera_ = id; };
    [[nodiscard]] std::shared_ptr<Camera> camera() const {
        return cameras_[activeCamera_];
    };

    void traverse( float frameDuration );

private:
    void initPhysics();
    void initScene();

private:
    nlohmann::json scene_;

    int activeCamera_{ 0 };

protected:
    float frameDuration_{};

    std::vector<std::shared_ptr<Body>> bodyList_{};
    std::vector<std::shared_ptr<Camera>> cameras_{};

    // JPH::PhysicsSystem system_{};
};

}  // namespace tire
