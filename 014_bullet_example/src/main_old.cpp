
#include <iostream>
#include <fmt/format.h>
#include <memory>
#include <thread>

#include "btBulletDynamicsCommon.h"

std::shared_ptr<btDefaultCollisionConfiguration> collisionConfiguration;
///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
std::shared_ptr<btCollisionDispatcher> dispatcher;
///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
std::shared_ptr<btBroadphaseInterface> overlappingPairCache;
///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
std::shared_ptr<btSequentialImpulseConstraintSolver> solver;
std::shared_ptr<btDiscreteDynamicsWorld> dynamicsWorld; 

void initPhysicWorld() {
	collisionConfiguration 	= std::make_shared<btDefaultCollisionConfiguration>();
	dispatcher				= std::make_shared<btCollisionDispatcher>(collisionConfiguration.get());
	overlappingPairCache	= std::make_shared<btDbvtBroadphase>();
	solver					= std::make_shared<btSequentialImpulseConstraintSolver>();
	dynamicsWorld			= std::make_shared<btDiscreteDynamicsWorld>(dispatcher.get(), overlappingPairCache.get(), solver.get(), collisionConfiguration.get());

	dynamicsWorld->setGravity(btVector3(0, -10, 0));
}

int main(int argc, char** argv) {
	initPhysicWorld();

	//Стоп на ~ 1/60 секунды
	std::this_thread::sleep_for(std::chrono::milliseconds(17));

	return 0;
}