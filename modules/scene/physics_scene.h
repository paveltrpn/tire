
#pragma once

#include <filesystem>
#include <memory>

#include <bullet/btBulletCollisionCommon.h>
#include <bullet/btBulletDynamicsCommon.h>
#include "BulletCollision/CollisionShapes/btCollisionShape.h"

#include "nlohmann/json.hpp"
#include "uv.h"

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

    std::unique_ptr<btDefaultCollisionConfiguration> collisionConfiguration_{};
    std::unique_ptr<btCollisionDispatcher> dispatcher_{};
    std::unique_ptr<btBroadphaseInterface> overlappingPairCache_{};
    std::unique_ptr<btSequentialImpulseConstraintSolver> solver_{};
    std::unique_ptr<btDiscreteDynamicsWorld> dynamicsWorld_{};

    int activeCamera_{ 0 };

protected:
    float frameDuration_{};

    std::vector<std::shared_ptr<Body>> bodyList_{};
    std::vector<btCollisionShape *> collisionShapesList_{};
    std::vector<std::shared_ptr<Camera>> cameras_{};
};

}  // namespace tire
